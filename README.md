# Snake
* Snake Game Made using GLFW and OpenGL in C++

## Build
  * **Makefile**
    * Build
    ```
    make build
    ```
    * Compile
    ```
    make compile
    ```
    * Run
    ```
    make run
    ```

  * **Without Makefile**
    * Build
    ```
    cmake -S . -B build
    cmake --build build
    ```
    * Run
    ```
    ./Snake.exe
    ```

  * **Ninja**
    * Build
    ```
    cmake -S . -B build -G Ninja
    cmake --build build
    ```
    * Run
    ```
    ./Snake.exe
    ```

## Progress
  ### 1. Project Setup
  * CMake
  * GLFW
  * pch

  ### 2. Logger
  * SN_INFO
  * SN_WARN
  * SN_ERROR
  * SN_ASSERT
  
  ### 3. Display
  * create
  * enable vsync
  * update
  * swapBuffers
  * close
  * cleanup
  * deltaTime
  * FPS
  
  ### 4. Inputs
  * **Keyboard**
      * key_down
      * key_up
      * key_pressed
      * key_released
    
  * **Mouse**
      * mouse_button_down
      * mouse_button_up
      * mouse_button_pressed
      * mouse_button_released
      * get_mouse_scroll_x
      * get_mouse_scroll_y
      * get_mouse_scroll
  
  ### 5. OpenGL Renderer
  * gl_init
  * gl_render
  * gl_cleanup

  ### 6. Texture Rendering
  * texelFetch
  * stbi_image.h

  ### 7. Sprite Rendering
  * SSBO(shader Storage Buffer Object)
  * RenderState
  * GameState

  ### 8. Matrix
  * Mat4
  * Projection Matrix

  ### 9. View Matrix
  * Camera
  * view Matrix

  ### 10. Added Animation
  * animIdx
  * animate

  ### 11. Map Rendering
  * MAP array
  * Map Sprites

  ### 12. Audio
  * Audio Engine
  * Sound
  * Music

  ### 13. Score
  * update score
  * spawn_bat

  ### 14. UI
  * draw_ui_sprite
  * draw_ui_quad

  ### 15. Font Rendering
  * load_fonts
  * render_ui_font
