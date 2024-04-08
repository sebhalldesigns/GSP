#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <emscripten.h>
#include <GLES2/gl2.h>

#include <GL/glfw.h>


#include <ft2build.h>
#include FT_FREETYPE_H

#define WIDTH  256
#define HEIGHT 256

// Define a function to generate a simple gradient in the pixel buffer
void generateGradient(uint32_t* buffer, int width, int height) {
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            uint8_t r = x * 255 / width;
            uint8_t g = y * 255 / height;
            uint8_t b = 0;
            buffer[y * width + x] = (r << 16) | (g << 8) | b;
        }
    }
}


// Function to initialize WebGL context
void initWebGL() {


    FT_Library library;
    FT_Face face;

    if (FT_Init_FreeType(&library)) {
        printf("Error initializing FreeType library\n");
    } else {
        printf("FREETYPE SETUP OK");
    }


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



            const vertexShaderSource = `
                attribute vec2 a_position;
                varying vec2 v_texCoord;
                void main() {
                gl_Position = vec4(a_position, 0.0, 1.0);
                v_texCoord = a_position * 0.5 + 0.5;
                }
            `;

            const fragmentShaderSource = `
                precision mediump float;
                varying vec2 v_texCoord;
                uniform sampler2D u_texture;
                void main() {
                gl_FragColor = texture2D(u_texture, v_texCoord);
            }
            `;

            const vertexShader = gl.createShader(gl.VERTEX_SHADER);
            gl.shaderSource(vertexShader, vertexShaderSource);
            gl.compileShader(vertexShader);

            const fragmentShader = gl.createShader(gl.FRAGMENT_SHADER);
            gl.shaderSource(fragmentShader, fragmentShaderSource);
            gl.compileShader(fragmentShader);

            const program = gl.createProgram();
            gl.attachShader(program, vertexShader);
            gl.attachShader(program, fragmentShader);
            gl.linkProgram(program);
            gl.useProgram(program);
            const positionBuffer = gl.createBuffer();
            gl.bindBuffer(gl.ARRAY_BUFFER, positionBuffer);
            const positions = [
                -1.0, -1.0,
                1.0, -1.0,
                -1.0,  1.0,
                -1.0,  1.0,
                1.0, -1.0,
                1.0,  1.0
            ];
            gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(positions), gl.STATIC_DRAW);

            const positionAttributeLocation = gl.getAttribLocation(program, 'a_position');
            gl.enableVertexAttribArray(positionAttributeLocation);
            gl.vertexAttribPointer(positionAttributeLocation, 2, gl.FLOAT, false, 0, 0);


        }
    });
}

// Function to render graphics using WebGL
void renderGraphics() {

    float red = 0.0f;
    float green = 1.0f;
    float blue = 1.0f;

    // Allocate memory for the pixel buffer
    uint32_t* buffer = (uint32_t*)malloc(WIDTH * HEIGHT * sizeof(uint32_t));

    // Generate a simple gradient in the pixel buffer
    generateGradient(buffer, WIDTH, HEIGHT);

   EM_ASM({
        // JavaScript code to render graphics with WebGL
        let gl = Module.webGLContext;


        

        const texture = gl.createTexture();
    gl.bindTexture(gl.TEXTURE_2D, texture);
     gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_S, gl.CLAMP_TO_EDGE);
    gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_T, gl.CLAMP_TO_EDGE);
    gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.NEAREST);
    gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MAG_FILTER, gl.NEAREST);
     gl.texImage2D(gl.TEXTURE_2D, 0, gl.RGBA, 256, 256, 0, gl.RGBA, gl.UNSIGNED_BYTE, new Uint8Array($3));

     // Your WebGL rendering code here
        // For example:
        gl.viewport(0, 0, gl.canvas.width, gl.canvas.height);
        gl.clearColor($0, $1, $2, 1.0);
        gl.clear(gl.COLOR_BUFFER_BIT);
         const textureLocation = gl.getUniformLocation(program, 'u_texture');
    gl.uniform1i(textureLocation, 0);

        gl.drawArrays(gl.TRIANGLES, 0, 6); // For example, draw a full-screen quad
    gl.deleteTexture(texture);


    }, red, green, blue, buffer);

 

    //free(buffer);
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