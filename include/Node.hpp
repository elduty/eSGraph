//
//  Node.hpp
//  eSGraph
//
//  Created by elduty on 27/07/2019.
//

#ifndef Node_h
#define Node_h

#define GLM_FORCE_XYZW_ONLY

#include <string>
#include <string_view>
#include <list>
#include <memory>
#include "glm/gtc/quaternion.hpp"

namespace eSGraph {
enum class Coordinates
{
    LOCAL,
    PARENT,
    WORLD
};

class Node
{
public:
    Node();
    explicit Node(std::string identifier);
    virtual ~Node() = default;

    [[nodiscard]] std::string_view getIdentifier() const noexcept;
    void setIdentifier(std::string_view identifier);

    void addChild(std::unique_ptr<Node> child);
    [[nodiscard]] std::unique_ptr<Node> removeChild(Node* child);
    [[nodiscard]] std::unique_ptr<Node> removeChild(std::string_view identifier);
    [[nodiscard]] std::list<std::unique_ptr<Node>> removeAllChildren();

    static void attachTo(std::unique_ptr<Node> node, Node* parent);
    [[nodiscard]] std::unique_ptr<Node> detach();

    [[nodiscard]] bool isChildOf(const Node* parent) const noexcept;
    [[nodiscard]] bool isChildOf(std::string_view identifier) const noexcept;
    [[nodiscard]] bool hasParent() const noexcept;
    [[nodiscard]] bool hasChild(const Node* child) const noexcept;
    [[nodiscard]] bool hasChild(std::string_view childIdentifier) const;
    [[nodiscard]] bool hasChildren() const noexcept;

    [[nodiscard]] Node* getParent() const noexcept;
    [[nodiscard]] const std::list<std::unique_ptr<Node>>& getChildren() const noexcept;

    void setPosition(const glm::vec3& position, Coordinates coordinates = Coordinates::LOCAL);
    [[nodiscard]] glm::vec3 getPosition(Coordinates coordinates = Coordinates::LOCAL) const;

    void setRotation(const glm::quat& rotation, Coordinates coordinates = Coordinates::LOCAL);
    void setRotation(const glm::vec3& eulerAngles, Coordinates coordinates = Coordinates::LOCAL);
    void setRotation(const glm::vec3& axis, float angle, Coordinates coordinates = Coordinates::LOCAL);
    [[nodiscard]] glm::quat getRotation(Coordinates coordinates = Coordinates::LOCAL) const;
    [[nodiscard]] glm::vec3 getEulerRotation(Coordinates coordinates = Coordinates::LOCAL) const;

    void setScale(const glm::vec3& scaleVector);
    void setScale(float x, float y, float z);
    void setScale(float scaleFactor);
    [[nodiscard]] const glm::vec3& getScale() const noexcept;

    [[nodiscard]] const glm::mat4& getMatrix();
    [[nodiscard]] const glm::mat4& getGlobalMatrix();

    void translate(const glm::vec3& translationVector, Coordinates coordinates = Coordinates::LOCAL);

    void rotate(const glm::vec3& eulers, Coordinates coordinates = Coordinates::LOCAL);
    void rotate(const glm::vec3& axis, float angle, Coordinates coordinates = Coordinates::LOCAL);
    void rotate(float xAngle, float yAngle, float zAngle, Coordinates coordinates = Coordinates::LOCAL);
    void rotate(const glm::quat& rotation, Coordinates coordinates = Coordinates::LOCAL);

    void scale(const glm::vec3& scaleVector);
    void scale(float x, float y, float z);
    void scale(float scaleFactor);

protected:
    glm::mat4 mMatrix{glm::identity<glm::mat4>()};
    glm::mat4 mGlobalMatrix{glm::identity<glm::mat4>()};
    glm::quat mRotation{glm::identity<glm::quat>()};
    glm::vec3 mPosition{0.0f};
    glm::vec3 mScale{1.0f};
    std::string mIdentifier;
    std::list<std::unique_ptr<Node>> mChildren;
    Node* mParent{nullptr};
    bool mGlobalMatrixDirty{true};
    bool mMatrixDirty{true};

    void setMatrixDirty();
    void setGlobalMatrixDirty();
};
}

#endif /* Node_h */
