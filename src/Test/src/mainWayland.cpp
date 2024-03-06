#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// X11 headers
#include <X11/Xlib.h>

// Wayland headers
#include <wayland-client.h>
#define _POSIX_C_SOURCE 200112L
#include <errno.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <time.h>
#include <unistd.h>
#include "xdg-shell-client-header.h"
#include "kde-server-decoration.h"

static void
randname(char *buf)
{
	struct timespec ts;
	clock_gettime(CLOCK_REALTIME, &ts);
	long r = ts.tv_nsec;
	for (int i = 0; i < 6; ++i) {
		buf[i] = 'A'+(r&15)+(r&16)*2;
		r >>= 5;
	}
}

static int
create_shm_file(void)
{
	int retries = 100;
	do {
		char name[] = "/wl_shm-XXXXXX";
		randname(name + sizeof(name) - 7);
		--retries;
		int fd = shm_open(name, O_RDWR | O_CREAT | O_EXCL, 0600);
		if (fd >= 0) {
			shm_unlink(name);
			return fd;
		}
	} while (retries > 0 && errno == EEXIST);
	return -1;
}

int
allocate_shm_file(size_t size)
{
	int fd = create_shm_file();
	if (fd < 0)
		return -1;
	int ret;
	do {
		ret = ftruncate(fd, size);
	} while (ret < 0 && errno == EINTR);
	if (ret < 0) {
		close(fd);
		return -1;
	}
	return fd;
}

struct WaylandState {
    wl_registry* registry;
    wl_compositor* compositor;
    wl_shm* shm;
    xdg_wm_base* xd_wm_base;

    wl_region* region;
    wl_surface* surface;
    xdg_surface* xd_surface;
    xdg_toplevel* xd_toplevel;
    org_kde_kwin_server_decoration_manager* kde_decorations;
};

static void
wl_buffer_release(void *data, struct wl_buffer *wl_buffer)
{
    /* Sent by the compositor when it's no longer using this buffer */
    wl_buffer_destroy(wl_buffer);
}

static const struct wl_buffer_listener wl_buffer_listener = {
    .release = wl_buffer_release,
};


static struct wl_buffer *
draw_frame(WaylandState*state)
{
    const int width = 640, height = 480;
    int stride = width * 4;
    int size = stride * height;

    int fd = allocate_shm_file(size);
    if (fd == -1) {
        return NULL;
    }

    uint32_t *data = (uint32_t*)mmap(NULL, size,
            PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (data == MAP_FAILED) {
        close(fd);
        return NULL;
    }

    struct wl_shm_pool *pool = wl_shm_create_pool(state->shm, fd, size);
    struct wl_buffer *buffer = wl_shm_pool_create_buffer(pool, 0,
            width, height, stride, WL_SHM_FORMAT_XRGB8888);
    wl_shm_pool_destroy(pool);
    close(fd);

    /* Draw checkerboxed background */
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if ((x + y / 8 * 8) % 16 < 8)
                data[y * width + x] = 0xFF666666;
            else
                data[y * width + x] = 0xFFEEEEEE;
        }
    }

    munmap(data, size);
    wl_buffer_add_listener(buffer, &wl_buffer_listener, NULL);
    return buffer;
}

// XDG STUFF vv
static void
xdg_wm_base_ping(void *data, struct xdg_wm_base *xdg_wm_base, uint32_t serial)
{
    xdg_wm_base_pong(xdg_wm_base, serial);
}

static const struct xdg_wm_base_listener xdg_wm_base_listener = {
    .ping = xdg_wm_base_ping,
};

static void
xdg_surface_configure(void *data,
        struct xdg_surface *xdg_surface, uint32_t serial)
{
    WaylandState* state = (WaylandState*)data;
    xdg_surface_ack_configure(xdg_surface, serial);

    wl_buffer* buffer = draw_frame(state);
    wl_surface_attach(state->surface, buffer, 0, 0);
    wl_surface_commit(state->surface);
}

static const struct xdg_surface_listener xdg_surface_listener = {
    .configure = xdg_surface_configure,
};

// XDG STUFF ^^


void registry_handle_global(void *data, struct wl_registry *registry, uint32_t name, const char *interface, uint32_t version) {
	printf("interface: '%s', version: %d, name: %d\n", interface, version, name);

    WaylandState* state = static_cast<WaylandState*>(data);

    if (strcmp(interface, wl_compositor_interface.name) == 0) {
        state->compositor = static_cast<wl_compositor*>(wl_registry_bind(state->registry, name, &wl_compositor_interface, 4));
    }

    if (strcmp(interface, wl_shm_interface.name) == 0) {
        state->shm = static_cast<wl_shm*>(wl_registry_bind(state->registry, name, &wl_shm_interface, 1));
    }

    if (strcmp(interface, xdg_wm_base_interface.name) == 0) {
        state->xd_wm_base = static_cast<xdg_wm_base*>(wl_registry_bind(state->registry, name, &xdg_wm_base_interface, 1));
        xdg_wm_base_add_listener(state->xd_wm_base,
                &xdg_wm_base_listener, state);
    }

    if (strcmp(interface, org_kde_kwin_server_decoration_manager_interface.name) == 0) {
        state->kde_decorations = static_cast<org_kde_kwin_server_decoration_manager*>(wl_registry_bind(state->registry, name, &org_kde_kwin_server_decoration_manager_interface, 1));
    }

}

void registry_handle_global_remove(void *data, struct wl_registry *registry, uint32_t name) {
	// This space deliberately left blank
}

const wl_registry_listener registry_listener = {
	.global = registry_handle_global,
	.global_remove = registry_handle_global_remove,
};

int main() {

    
    wl_display* waylandDisplay = nullptr;
    Display* xDisplay = nullptr;

    // try connecting to wayland first
    waylandDisplay = wl_display_connect(nullptr);

    if (waylandDisplay == nullptr) {
        xDisplay = XOpenDisplay(nullptr);
    }

    if (waylandDisplay != nullptr) {
        printf("Running on Wayland\n");
    } else if (xDisplay != nullptr) {
        printf("Running on X11\n");
    } else {
        printf("No display server found!\n");
        return -1;
    }
    
    // assuming wayland

    WaylandState waylandState = { 0 };

    waylandState.registry = wl_display_get_registry(waylandDisplay);

    wl_registry_add_listener(waylandState.registry, &registry_listener, &waylandState);
	wl_display_roundtrip(waylandDisplay);

    waylandState.surface = wl_compositor_create_surface(waylandState.compositor);

    waylandState.xd_surface = xdg_wm_base_get_xdg_surface(
            waylandState.xd_wm_base, waylandState.surface);

    xdg_surface_add_listener(waylandState.xd_surface, &xdg_surface_listener, &waylandState);

    /*const int width = 1920, height = 1080;
    const int stride = width * 4;
    const int shm_pool_size = height * stride * 2;

    int fd = allocate_shm_file(shm_pool_size);
    uint8_t *pool_data = static_cast<uint8_t*>(mmap(NULL, shm_pool_size,
        PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0));

    struct wl_shm_pool *pool = wl_shm_create_pool(waylandState.shm, fd, shm_pool_size);
    int index = 0;
    int offset = height * stride * index;
    struct wl_buffer *buffer = wl_shm_pool_create_buffer(pool, offset,
        width, height, stride, WL_SHM_FORMAT_XRGB8888);

    uint32_t *pixels = (uint32_t *)&pool_data[offset];
    memset(pixels, 0, width * height * 4);*/

    waylandState.xd_toplevel = xdg_surface_get_toplevel(waylandState.xd_surface);
    xdg_toplevel_set_title(waylandState.xd_toplevel, "Example client");
    org_kde_kwin_server_decoration_manager_create(waylandState.kde_decorations, waylandState.surface);

    wl_surface_commit(waylandState.surface);

    while (wl_display_dispatch(waylandDisplay)) {
        /* This space deliberately left blank */
    }

    wl_display_disconnect(waylandDisplay);
    return 0;
}