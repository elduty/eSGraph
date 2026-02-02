//
//  Node.cpp
//  eSGraph
//
//  Created by elduty on 27/07/2019.
//

#include "Node.hpp"
#include <vector>
#include <cmath>

using namespace eSGraph;

Node::Node() = default;

Node::Node(std::string identifier)
    : mIdentifier{std::move(identifier)}
{
}

std::string_view Node::getIdentifier() const noexcept
{
    return mIdentifier;
}

void Node::setIdentifier(std::string_view identifier)
{
    mIdentifier = identifier;
}

bool Node::isChildOf(const Node* parent) const noexcept
{
    return mParent == parent;
}

bool Node::isChildOf(std::string_view identifier) const noexcept
{
    return hasParent() && mParent->getIdentifier() == identifier;
}

bool Node::hasChildren() const noexcept
{
    return !mChildren.empty();
}

bool Node::hasChild(const Node* child) const noexcept
{
    return child != nullptr && child->getParent() == this;
}

bool Node::hasChild(std::string_view childIdentifier) const
{
    for (const auto& childrenElement : mChildren)
    {
        if (childrenElement->getIdentifier() == childIdentifier)
        {
            return true;
        }
    }
    return false;
}

bool Node::hasParent() const noexcept
{
    return mParent != nullptr;
}

void Node::addChild(std::unique_ptr<Node> child)
{
    assert(!child->hasParent());

    child->mParent = this;
    child->setGlobalMatrixDirty();
    mChildren.push_back(std::move(child));
}

std::unique_ptr<Node> Node::removeChild(std::string_view identifier)
{
    assert(hasChild(identifier));

    for (auto& childrenElement : mChildren)
    {
        if (childrenElement->getIdentifier() == identifier)
        {
            return removeChild(childrenElement.get());
        }
    }
    return nullptr;
}

std::unique_ptr<Node> Node::removeChild(Node* child)
{
    std::unique_ptr<Node> returnElement;
    if (child->isChildOf(this))
    {
        child->mParent = nullptr;
        child->setGlobalMatrixDirty();

        for (auto& childrenElement : mChildren)
        {
            if (childrenElement.get() == child)
            {
                returnElement = std::move(childrenElement);
                mChildren.remove(childrenElement);
                break;
            }
        }
    }
    return returnElement;
}

void Node::attachTo(std::unique_ptr<Node> node, Node* parent)
{
    assert(node && !node->hasParent());
    parent->addChild(std::move(node));
}

std::unique_ptr<Node> Node::detach()
{
    assert(hasParent());
    return mParent->removeChild(this);
}

std::list<std::unique_ptr<Node>> Node::removeAllChildren()
{
    for (auto& child : mChildren)
    {
        child->mParent = nullptr;
        child->setGlobalMatrixDirty();
    }
    return std::move(mChildren);
}

Node* Node::getParent() const noexcept
{
    return mParent;
}

const std::list<std::unique_ptr<Node>>& Node::getChildren() const noexcept
{
    return mChildren;
}

Node* Node::findByIdentifier(std::string_view identifier) const
{
    for (const auto& child : mChildren)
    {
        if (child->getIdentifier() == identifier)
        {
            return child.get();
        }
        if (Node* found = child->findByIdentifier(identifier))
        {
            return found;
        }
    }
    return nullptr;
}

Node* Node::getRoot() noexcept
{
    Node* current = this;
    while (current->mParent != nullptr)
    {
        current = current->mParent;
    }
    return current;
}

const Node* Node::getRoot() const noexcept
{
    const Node* current = this;
    while (current->mParent != nullptr)
    {
        current = current->mParent;
    }
    return current;
}

size_t Node::getDepth() const noexcept
{
    size_t depth = 0;
    const Node* current = this;
    while (current->mParent != nullptr)
    {
        ++depth;
        current = current->mParent;
    }
    return depth;
}

Node* Node::getNextSibling() const noexcept
{
    if (!mParent)
    {
        return nullptr;
    }

    const auto& siblings = mParent->mChildren;
    for (auto it = siblings.begin(); it != siblings.end(); ++it)
    {
        if (it->get() == this)
        {
            ++it;
            if (it != siblings.end())
            {
                return it->get();
            }
            return nullptr;
        }
    }
    return nullptr;
}

Node* Node::getPreviousSibling() const noexcept
{
    if (!mParent)
    {
        return nullptr;
    }

    const auto& siblings = mParent->mChildren;
    Node* previous = nullptr;
    for (const auto& sibling : siblings)
    {
        if (sibling.get() == this)
        {
            return previous;
        }
        previous = sibling.get();
    }
    return nullptr;
}

glm::vec3 Node::getPosition(Coordinates coordinates) const
{
    switch (coordinates) {
        case Coordinates::WORLD:
            if (hasParent())
                return mParent->getGlobalMatrix() * glm::vec4(mPosition, 1.0);
            [[fallthrough]];
        case Coordinates::PARENT:
        case Coordinates::LOCAL:
        default:
            return mPosition;
    }
}

void Node::setPosition(const glm::vec3& position, Coordinates coordinates)
{
    switch (coordinates) {
        case Coordinates::WORLD:
            if (hasParent())
            {
                mPosition = glm::inverse(mParent->getGlobalMatrix()) * glm::vec4(position, 1.0f);
                break;
            }
            [[fallthrough]];
        case Coordinates::PARENT:
        case Coordinates::LOCAL:
        default:
            mPosition = position;
            break;
    }
    setMatrixDirty();
}

const glm::vec3& Node::getScale() const noexcept
{
    return mScale;
}

void Node::setScale(const glm::vec3& scaleVector)
{
    mScale = scaleVector;
    setMatrixDirty();
}

void Node::setScale(float x, float y, float z)
{
    setScale(glm::vec3(x, y, z));
}

void Node::setScale(float scaleFactor)
{
    setScale(glm::vec3(scaleFactor));
}

glm::quat Node::getRotation(Coordinates coordinates) const
{
    switch (coordinates) {
        case Coordinates::WORLD:
            if (hasParent())
            {
                return mParent->getRotation(Coordinates::WORLD) * mRotation;
            }
            [[fallthrough]];
        case Coordinates::PARENT:
        case Coordinates::LOCAL:
        default:
            return mRotation;
    }
}

glm::vec3 Node::getEulerRotation(Coordinates coordinates) const
{
    return glm::eulerAngles(getRotation(coordinates));
}

void Node::setRotation(const glm::vec3& axis, float angle, Coordinates coordinates)
{
    setRotation(glm::angleAxis(angle, glm::normalize(axis)), coordinates);
}

void Node::setRotation(const glm::quat& rotation, Coordinates coordinates)
{
    switch (coordinates) {
        case Coordinates::WORLD:
            if (hasParent())
            {
                mRotation = glm::conjugate(mParent->getRotation(Coordinates::WORLD)) * glm::normalize(rotation);
                break;
            }
            [[fallthrough]];
        case Coordinates::PARENT:
        case Coordinates::LOCAL:
        default:
            mRotation = glm::normalize(rotation);
            break;
    }
    setMatrixDirty();
}

void Node::setRotation(const glm::vec3& eulerAngles, Coordinates coordinates)
{
    setRotation(glm::quat(eulerAngles), coordinates);
}

void Node::setMatrixDirty()
{
    mMatrixDirty = true;
    setGlobalMatrixDirty();
}

void Node::setGlobalMatrixDirty()
{
    std::vector<Node*> stack;
    stack.push_back(this);

    while (!stack.empty())
    {
        Node* current = stack.back();
        stack.pop_back();

        current->mGlobalMatrixDirty = true;

        for (auto& child : current->mChildren)
        {
            stack.push_back(child.get());
        }
    }
}

const glm::mat4& Node::getMatrix()
{
    if (mMatrixDirty)
    {
        mMatrix = glm::translate(glm::identity<glm::mat4>(), mPosition) *
                  glm::mat4_cast(mRotation) *
                  glm::scale(glm::identity<glm::mat4>(), mScale);
        mMatrixDirty = false;
    }
    return mMatrix;
}

const glm::mat4& Node::getGlobalMatrix()
{
    if (mGlobalMatrixDirty)
    {
        if (!hasParent())
            mGlobalMatrix = getMatrix();
        else
            mGlobalMatrix = mParent->getGlobalMatrix() * getMatrix();

        mGlobalMatrixDirty = false;
    }
    return mGlobalMatrix;
}

void Node::translate(const glm::vec3& translationVector, Coordinates coordinates)
{
    switch (coordinates) {
        case Coordinates::WORLD:
            if (hasParent())
            {
                glm::vec3 currentWorldPos = getPosition(Coordinates::WORLD);
                setPosition(currentWorldPos + translationVector, Coordinates::WORLD);
            }
            else
            {
                setPosition(mPosition + translationVector);
            }
            break;
        case Coordinates::PARENT:
            setPosition(mPosition + translationVector);
            break;
        case Coordinates::LOCAL:
        default:
            setPosition(mPosition + mRotation * translationVector);
            break;
    }
}

void Node::scale(const glm::vec3& scaleVector)
{
    setScale(mScale * scaleVector);
}

void Node::scale(float x, float y, float z)
{
    scale(glm::vec3(x, y, z));
}

void Node::scale(float scaleFactor)
{
    scale(glm::vec3(scaleFactor));
}

void Node::rotate(const glm::quat& rotation, Coordinates coordinates)
{
    rotate(glm::axis(rotation), glm::angle(rotation), coordinates);
}

void Node::rotate(float xAngle, float yAngle, float zAngle, Coordinates coordinates)
{
    rotate(glm::vec3(xAngle, yAngle, zAngle), coordinates);
}

void Node::rotate(const glm::vec3& euler, Coordinates coordinates)
{
    rotate(glm::vec3(1.0f, 0.0f, 0.0f), euler.x, coordinates);
    rotate(glm::vec3(0.0f, 1.0f, 0.0f), euler.y, coordinates);
    rotate(glm::vec3(0.0f, 0.0f, 1.0f), euler.z, coordinates);
}

void Node::rotate(const glm::vec3& axis, float angle, Coordinates coordinates)
{
    switch (coordinates) {
        case Coordinates::WORLD:
        {
            glm::vec3 localAxis = glm::conjugate(getRotation(Coordinates::WORLD)) * glm::normalize(axis);
            setRotation(glm::rotate(mRotation, angle, localAxis));
            break;
        }
        case Coordinates::PARENT:
        {
            glm::vec3 localAxis = glm::conjugate(mRotation) * glm::normalize(axis);
            setRotation(glm::rotate(mRotation, angle, localAxis));
            break;
        }
        case Coordinates::LOCAL:
        default:
            setRotation(glm::rotate(mRotation, angle, glm::normalize(axis)));
            break;
    }
}

glm::vec3 Node::getForward(Coordinates coordinates) const
{
    glm::quat rotation = getRotation(coordinates);
    return rotation * glm::vec3(0.0f, 0.0f, -1.0f);
}

glm::vec3 Node::getRight(Coordinates coordinates) const
{
    glm::quat rotation = getRotation(coordinates);
    return rotation * glm::vec3(1.0f, 0.0f, 0.0f);
}

glm::vec3 Node::getUp(Coordinates coordinates) const
{
    glm::quat rotation = getRotation(coordinates);
    return rotation * glm::vec3(0.0f, 1.0f, 0.0f);
}

void Node::lookAt(const glm::vec3& target, const glm::vec3& up)
{
    glm::vec3 worldPos = getPosition(Coordinates::WORLD);
    glm::vec3 direction = glm::normalize(target - worldPos);

    if (glm::length(direction) < 1e-6f)
    {
        return;
    }

    glm::vec3 right = glm::normalize(glm::cross(up, -direction));
    glm::vec3 correctedUp = glm::cross(-direction, right);

    glm::mat3 rotationMatrix(right, correctedUp, -direction);
    glm::quat worldRotation = glm::quat_cast(rotationMatrix);

    setRotation(worldRotation, Coordinates::WORLD);
}

std::unique_ptr<Node> Node::clone() const
{
    auto cloned = std::make_unique<Node>(mIdentifier);

    cloned->mPosition = mPosition;
    cloned->mRotation = mRotation;
    cloned->mScale = mScale;
    cloned->mMatrixDirty = true;
    cloned->mGlobalMatrixDirty = true;

    for (const auto& child : mChildren)
    {
        cloned->addChild(child->clone());
    }

    return cloned;
}
