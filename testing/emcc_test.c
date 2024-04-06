#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <emscripten.h>
#include <GLES2/gl2.h>

#include <GL/glfw.h>

// Function to initialize WebGL context
void initWebGL() {
    EM_ASM({
        // JavaScript code to initialize WebGL
        var canvas = document.getElementById('canvas');
        var gl = canvas.getContext('webgl');

        if (!gl) {
            console.error('WebGL not supported, falling back on experimental-webgl');
            gl = canvas.getContext('experimental-webgl');
        }

        if (!gl) {
            alert('Your browser does not support WebGL');
        } else {
            console.log('WebGL context initialized successfully');
            // Store WebGL context for later use
            Module.webGLContext = gl;
        }
    });
}

// Function to render graphics using WebGL
void renderGraphics() {

    float red = 0.0f;
    float green = 1.0f;
    float blue = 0.0f;


    EM_ASM({
        // JavaScript code to render graphics with WebGL
        var gl = Module.webGLContext;

        // Your WebGL rendering code here
        // For example:
        gl.clearColor($0, $1, $2, 1.0);
        gl.clear(gl.COLOR_BUFFER_BIT);
    }, red, green, blue);
}

void mainLoop() {
    printf("Hello world from GSP\n");
    renderGraphics();
}

int main(){
        
        initWebGL();
        emscripten_set_main_loop(&mainLoop, 0, 1);

    return 0;
}