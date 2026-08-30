#include "Camera.h"

Camera::Camera(int width, int height, glm::vec3 position) {
    Camera::width = width;
    Camera::height = height;
    Camera::Position = position;
}

void Camera::updateMatrix(float FOVdeg, float nearPlane, float farPlane) {
    // Initializes matrices
    auto view = glm::mat4(1.0f);
    auto projection = glm::mat4(1.0f);

    // Makes camera look in the right direction
    view = glm::lookAt(Position, Position + Orientation, Up);
    // Adds perspective to the scene
    projection = glm::perspective(glm::radians(FOVdeg), static_cast<float>(width) / height, nearPlane, farPlane);

    // Sets new camera matrix
    cameraMatrix = projection * view;
}

void Camera::exportMatrixToShader(Shader& shader, const char* uniform) {
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform), 1, GL_FALSE, glm::value_ptr(cameraMatrix));
}

void Camera::exportPositionToShader(Shader& shader, const char* uniform) const {
    glUniform3f(glGetUniformLocation(shader.ID, uniform), Position.x, Position.y, Position.z);
}

void Camera::handleInputs(GLFWwindow* window, float deltaTime) {
    float newSpeed = speed * deltaTime;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        Position += newSpeed * Orientation;
    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        Position += newSpeed * -glm::normalize(glm::cross(Orientation, Up));
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        Position += newSpeed * -Orientation;
    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        Position += newSpeed * glm::normalize(glm::cross(Orientation, Up));
    }

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        Position += newSpeed * Up;
    }

    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
        Position += newSpeed * -Up;
    }

    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        speed = fastSpeed;
    } else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE) {
        speed = normalSpeed;
    }

    // Mouse inputs
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        // Hide mouse cursor
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

        // Prevents camera from jumping on the first click
        if (firstClick) {
            glfwSetCursorPos(window, (width / 2), (height / 2));
            firstClick = false;
        }

        // Get the coordinates of the cursor
        double mouseX;
        double mouseY;
        glfwGetCursorPos(window, &mouseX, &mouseY);

        float rotX = sensitivity * static_cast<float>(mouseY - (height / 2)) / height;
        float rotY = sensitivity * static_cast<float>(mouseX - (width / 2)) / width;

        glm::vec3 newOrientation = glm::rotate(Orientation, glm::radians(-rotX), glm::normalize(glm::cross(Orientation, Up)));

        // Only allow the camera to rotate up and down 85 degrees
        if (abs(glm::angle(newOrientation, Up) - glm::radians(90.0f)) <= glm::radians(85.0f)) {
            Orientation = newOrientation;
        }

        // Rotates the Orientation left and right
        Orientation = glm::rotate(Orientation, glm::radians(-rotY), Up);

        // Sets mouse cursor to the middle of the screen
        glfwSetCursorPos(window, (width / 2), (height / 2));
    } else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) {
        // Unhides cursor
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        firstClick = true;
    }
}
