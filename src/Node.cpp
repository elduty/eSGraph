//
//  Node.cpp
//  eSGraph
//
//  Created by elduty on 27/07/2019.
//

#include "Node.hpp"

using namespace eSGraph;

Node::Node() : mParent(nullptr), mMatrix(glm::identity<glm::mat4>()),mGlobalMatrix(glm::identity<glm::mat4>()), mPosition(glm::vec3()), mScale(glm::vec3(1.0)), mRotation(glm::identity<glm::quat>()), mGlobalMatrixDirty(true), mMatrixDirty(true)
{
}

Node::Node(const std::string& identifier): Node()
{
    mIdentifier = identifier;
}

Node::~Node()
{
}

std::string Node::getIdentifier() const
{
    return mIdentifier;
}

void Node::setIdentifier(const std::string& identifier)
{
    mIdentifier = identifier;
}

bool Node::isChildOf(Node* parent) const
{
    return mParent == parent;
}

bool Node::isChildOf(const std::string& identifier) const
{
    return hasParent() && mParent->getIdentifier() == identifier;
}

bool Node::hasChildren() const
{
    return !mChildren.empty();
}

bool Node::hasChild(Node* child) const
{
    return child->getParent() == this;
}

bool Node::hasChild(const std::string& childIdentifier) const
{
    for(auto&& childrenElement : mChildren)
    {
        if(childrenElement.get()->getIdentifier() == childIdentifier)
        {
            return true;
        }
    }
    
    return false;
}

bool Node::hasParent() const
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

std::unique_ptr<Node> Node::removeChild(const std::string& identifier)
{
    assert(hasChild(identifier));
    
    std::unique_ptr<Node> removedChild = nullptr;
    
    for(auto&& childrenElement : mChildren)
    {
        if(childrenElement.get()->getIdentifier() == identifier)
        {
            removedChild = removeChild(childrenElement.get());
            break;
        }
    }
    
    return removedChild;
}

std::unique_ptr<Node> Node::removeChild(Node* child)
{
    std::unique_ptr<Node> returnElement = nullptr;
    if(child->isChildOf(this))
    {
        child->mParent = nullptr;
        child->setGlobalMatrixDirty();
        
        for(auto&& childrenElement : mChildren)
        {
            if(childrenElement.get() == child)
            {
                returnElement = std::move(childrenElement);
                mChildren.remove(childrenElement);
                break;
            }
        }
    }
    
    return returnElement;
}

void Node::attach(Node* parent)
{
    assert(!hasParent());
    parent->addChild(std::unique_ptr<Node>(this));
}

std::unique_ptr<Node> Node::detach()
{
    assert(hasParent());
    return mParent->removeChild(this);
}

std::list<std::unique_ptr<Node>> Node::removeAllChildren()
{
    for(auto&& child : mChildren)
    {
        child->mParent = nullptr;
        child->setGlobalMatrixDirty();
    }
    
    return std::move(mChildren);
}

Node* Node::getParent() const
{
    return mParent;
}

const std::list<std::unique_ptr<Node>>& Node::getChildren() const
{
    return mChildren;
}

glm::vec3 Node::getPosition(Coordinates coordinates) const
{
    switch (coordinates) {
        case Coordinates::WORLD:
            if(hasParent())
                return mParent->getGlobalMatrix() * glm::vec4(mPosition, 1.0);
        case Coordinates::LOCAL:
            return mPosition;
    }
}

void Node::setPosition(const glm::vec3& position, Coordinates coordinates)
{
    switch (coordinates) {
        case Coordinates::WORLD:
            if(hasParent())
            {
                mPosition = glm::inverse(mParent->getGlobalMatrix()) * glm::vec4(position, 1.0f);
                break;
            }
        case Coordinates::LOCAL:
            mPosition = position;
            break;
    }
    
    mMatrixDirty = true;
    setGlobalMatrixDirty();
}

glm::vec3 Node::getScale() const
{
    return mScale;
}

void Node::setScale(const glm::vec3& scaleVector)
{
    mScale = scaleVector;
    mMatrixDirty = true;
    setGlobalMatrixDirty();
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
            if(hasParent())
                return mParent->getGlobalMatrix() * glm::mat4_cast(mRotation);
        case Coordinates::LOCAL:
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
            if(hasParent())
            {
                mRotation = glm::inverse(mParent->getGlobalMatrix()) * glm::mat4_cast(glm::normalize(rotation));
                break;
            }
        case Coordinates::LOCAL:
            mRotation = glm::normalize(rotation);
            break;
    }
    mMatrixDirty = true;
    setGlobalMatrixDirty();
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
    mGlobalMatrixDirty = true;
    
    for(auto&& child : mChildren)
    {
        child->setGlobalMatrixDirty();
    }
}

glm::mat4 Node::getMatrix()
{
    if(mMatrixDirty)
    {
        mMatrix = glm::translate(glm::identity<glm::mat4>(), mPosition) * glm::mat4_cast(mRotation) * glm::scale(glm::identity<glm::mat4>(), mScale);
        
        mMatrixDirty = false;
    }
    return mMatrix;
}

glm::mat4 Node::getGlobalMatrix()
{
    if(mGlobalMatrixDirty)
    {
        if(!hasParent())
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
            if(hasParent())
            {
                setPosition(mPosition + glm::inverse(mParent->getRotation(Coordinates::WORLD)) * translationVector);
            }else
            {
                setPosition(mPosition + translationVector);
            }
            break;
        case Coordinates::LOCAL:
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
    rotate(glm::vec3(1.0f, 0.0f, 0.0f), euler.x);
    rotate(glm::vec3(0.0f, 1.0f, 0.0f), euler.y);
    rotate(glm::vec3(0.0f, 0.0f, 1.0f), euler.z);
}

void Node::rotate(const glm::vec3& axis, float angle, Coordinates coordinates)
{
    switch (coordinates) {
        case Coordinates::WORLD:
                setRotation(glm::rotate(mRotation, angle, glm::normalize(axis) * getRotation(Coordinates::WORLD)));
                break;
        case Coordinates::LOCAL:
            setRotation(glm::rotate(mRotation, angle, glm::normalize(axis)));
            break;
    }
}
