#pragma once

#include <list>
#include <future>

#include <glm/mat4x4.hpp>

#include "ao/gl/core.hpp"
#include "ao/ui/task.hpp"

class Tree;

/*
 *  The Frame class contains and draws many rendered Tree textures
 */
class Frame
{
public:
    /*
     *  Constructor and destructor
     *
     *  On construction, takes ownership of the given Tree and sets
     *  its parent pointer.
     */
    explicit Frame(Tree* tree);
    ~Frame();

    /*
     *  Draws the set of textures with the given matrix applied
     */
    void draw(const glm::mat4& m) const;

    /*
     *  Pushes a new render task to the stack at the given matrix
     */
    void render(const glm::mat4& m, size_t ni, size_t nj, size_t nk);

    /*
     *  Check the future watcher and see if it's ready
     *  Returns true if the parent window should redraw
     */
    bool poll();

    /*
     *  Returns true if the render thread is still running
     */
    bool running() const;

protected:
    /*
     *  Kicks off an async render task if there is a pending valid task
     *
     *  Requires the future to be invalid when called
     */
    void startRender();

    /*
     *  Represents a render task
     */

    std::unique_ptr<Tree> const tree;

    GLuint vs;  // Vertex shader
    GLuint fs;  // Fragment shader
    GLuint prog;    // Shader program

    GLuint vbo; // Vertex buffer object
    GLuint vao; // Vertex array object

    GLuint depth;  // Depth texture
    GLuint norm;   // Normals texture

    // Completed render task
    Task current;

    // Active render task and worker
    Task pending;
    std::unique_ptr<Worker> worker;

    // Next render task
    Task next;

    // Shader source strings
    static const std::string vert;
    static const std::string frag;
};
