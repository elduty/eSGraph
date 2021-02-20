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
#include <list>
#include "glm/gtc/quaternion.hpp"

namespace eSGraph {
enum class Coordinates
{
    LOCAL,
    //PARENT,
    WORLD
};
    
class Node
{
public:
    Node();
    Node(const std::string& identifier);
    virtual ~Node();
    
    std::string getIdentifier() const;
    void setIdentifier(const std::string& identifier);
    
    void addChild(const std::unique_ptr<Node> child);
    std::unique_ptr<Node> removeChild(Node* child);
    std::unique_ptr<Node> removeChild(const std::string& identifier);
    std::list<std::unique_ptr<Node>> removeAllChildren();
    
    void attach(Node* parent);
    std::unique_ptr<Node> detach();
    
    bool isChildOf(Node* parent) const;
    bool isChildOf(const std::string& identifier) const;
    bool hasParent() const;
    bool hasChild(Node* child) const;
    bool hasChild(const std::string& childIdentifier) const;
    bool hasChildren() const;
    
    Node* getParent() const;
    const std::list<std::unique_ptr<Node>>& getChildren() const;
    
    void setPosition(const glm::vec3& position, Coordinates coordinates = Coordinates::LOCAL);
    glm::vec3 getPosition(Coordinates coordinates = Coordinates::LOCAL) const;
    
    void setRotation(const glm::quat& rotation, Coordinates coordinates = Coordinates::LOCAL);
    void setRotation(const glm::vec3& eulerAngles, Coordinates coordinates = Coordinates::LOCAL);
    void setRotation(const glm::vec3& axis, float angle, Coordinates coordinates = Coordinates::LOCAL);
    glm::quat getRotation(Coordinates coordinates = Coordinates::LOCAL) const;
    glm::vec3 getEulerRotation(Coordinates coordinates = Coordinates::LOCAL) const;
    
    void setScale(const glm::vec3& scaleVector);
    void setScale(float x, float y, float z);
    void setScale(float scaleFactor);
    const glm::vec3& getScale() const;
    
    const glm::mat4& getMatrix();
    const glm::mat4& getGlobalMatrix();
    
    void translate(const glm::vec3& translationVector, Coordinates coordinates = Coordinates::LOCAL);
    
    void rotate(const glm::vec3& eulers, Coordinates coordinates = Coordinates::LOCAL);
    void rotate(const glm::vec3& axis, float angle, Coordinates coordinates = Coordinates::LOCAL);
    void rotate(float xAngle, float yAngle, float zAngle, Coordinates coordinates = Coordinates::LOCAL);
    void rotate(const glm::quat& rotation, Coordinates coordinates = Coordinates::LOCAL);
    
    void scale(const glm::vec3& scaleVector);
    void scale(float x, float y, float z);
    void scale(float scaleFactor);
    
protected:
    std::string mIdentifier;
    std::list<std::unique_ptr<Node>> mChildren;
    Node* mParent;
    bool mGlobalMatrixDirty;
    bool mMatrixDirty;
    glm::mat4 mMatrix;
    glm::mat4 mGlobalMatrix;
    glm::vec3 mPosition;
    glm::vec3 mScale;
    glm::quat mRotation;
    
    void setMatrixDirty();
    void setGlobalMatrixDirty();
};
}

#endif /* Node_h */
