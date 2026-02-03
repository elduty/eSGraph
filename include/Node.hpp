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
#include <vector>
#include <memory>
#include <functional>
#include "glm/gtc/quaternion.hpp"

namespace eSGraph {
enum class Coordinates
{
    LOCAL,
    PARENT,
    WORLD
};

struct DirectionVectors
{
    glm::vec3 forward;
    glm::vec3 right;
    glm::vec3 up;
};

class Node
{
public:
    Node();
    explicit Node(std::string identifier);
    virtual ~Node() = default;

    [[nodiscard]] std::string_view getIdentifier() const noexcept { return mIdentifier; }
    void setIdentifier(std::string_view identifier);

    void addChild(std::unique_ptr<Node> child);
    [[nodiscard]] std::unique_ptr<Node> removeChild(Node* child);
    [[nodiscard]] std::unique_ptr<Node> removeChild(std::string_view identifier);
    [[nodiscard]] std::vector<std::unique_ptr<Node>> removeAllChildren();

    static void attachTo(std::unique_ptr<Node> node, Node* parent);
    [[nodiscard]] std::unique_ptr<Node> detach();

    [[nodiscard]] bool isChildOf(const Node* parent) const noexcept;
    [[nodiscard]] bool isChildOf(std::string_view identifier) const noexcept;
    [[nodiscard]] bool hasParent() const noexcept { return mParent != nullptr; }
    [[nodiscard]] bool hasChild(const Node* child) const noexcept;
    [[nodiscard]] bool hasChild(std::string_view childIdentifier) const;
    [[nodiscard]] bool hasChildren() const noexcept { return !mChildren.empty(); }

    [[nodiscard]] Node* getParent() const noexcept { return mParent; }
    [[nodiscard]] const std::vector<std::unique_ptr<Node>>& getChildren() const noexcept;

    // Hierarchy traversal
    [[nodiscard]] Node* findByIdentifier(std::string_view identifier) const;
    [[nodiscard]] Node* getRoot() noexcept;
    [[nodiscard]] const Node* getRoot() const noexcept;
    [[nodiscard]] size_t getDepth() const noexcept;
    [[nodiscard]] Node* getNextSibling() const noexcept;
    [[nodiscard]] Node* getPreviousSibling() const noexcept;

    template<typename Visitor>
    void traverse(Visitor&& visitor);

    template<typename Visitor>
    void traverse(Visitor&& visitor) const;

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
    [[nodiscard]] const glm::vec3& getScale() const noexcept { return mScale; }

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

    // Direction vectors
    [[nodiscard]] glm::vec3 getForward(Coordinates coordinates = Coordinates::WORLD) const;
    [[nodiscard]] glm::vec3 getRight(Coordinates coordinates = Coordinates::WORLD) const;
    [[nodiscard]] glm::vec3 getUp(Coordinates coordinates = Coordinates::WORLD) const;
    [[nodiscard]] DirectionVectors getDirections(Coordinates coordinates = Coordinates::WORLD) const;

    // LookAt
    void lookAt(const glm::vec3& target, const glm::vec3& up = glm::vec3(0.0f, 1.0f, 0.0f));

    // Clone
    [[nodiscard]] std::unique_ptr<Node> clone() const;

protected:
    // Hot path - checked frequently
    Node* mParent{nullptr};
    bool mMatrixDirty{true};
    bool mGlobalMatrixDirty{true};
    mutable bool mWorldRotationDirty{true};

    // Transform data
    glm::vec3 mPosition{0.0f};
    glm::quat mRotation{glm::identity<glm::quat>()};
    glm::vec3 mScale{1.0f};

    // Cached matrices and rotations (large, less frequent writes)
    glm::mat4 mMatrix{glm::identity<glm::mat4>()};
    glm::mat4 mGlobalMatrix{glm::identity<glm::mat4>()};
    mutable glm::quat mWorldRotation{glm::identity<glm::quat>()};

    // Cold data
    std::string mIdentifier;
    std::vector<std::unique_ptr<Node>> mChildren;

    void setMatrixDirty();
    void setGlobalMatrixDirty();
    [[nodiscard]] const glm::quat& getWorldRotationCached() const;
};

// Template implementations
template<typename Visitor>
void Node::traverse(Visitor&& visitor)
{
    visitor(*this);
    for (auto& child : mChildren)
    {
        child->traverse(std::forward<Visitor>(visitor));
    }
}

template<typename Visitor>
void Node::traverse(Visitor&& visitor) const
{
    visitor(*this);
    for (const auto& child : mChildren)
    {
        child->traverse(std::forward<Visitor>(visitor));
    }
}

}

#endif /* Node_h */
