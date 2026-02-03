//
//  NodeTests.cpp
//  eSGraph
//
//  Created by elduty on 03/08/2019.
//

#include "NodeTests.hpp"
#include "Node.hpp"
#include <memory>
#include <cmath>
#include <gtest/gtest.h>

using namespace eSGraph;

namespace {

void EXPECT_VEC3_NEAR(const glm::vec3& a, const glm::vec3& b, float eps = 1e-5f) {
    EXPECT_NEAR(a.x, b.x, eps);
    EXPECT_NEAR(a.y, b.y, eps);
    EXPECT_NEAR(a.z, b.z, eps);
}

void EXPECT_QUAT_NEAR(const glm::quat& a, const glm::quat& b, float eps = 1e-5f) {
    float dot = std::abs(glm::dot(a, b));
    EXPECT_GT(dot, 1.0f - eps);
}

}

void NodeTests::SetUp()
{
}

void NodeTests::TearDown()
{
}

TEST_F(NodeTests, checkTranslate)
{
    std::unique_ptr<Node> node = std::make_unique<Node>();
    
    node->setPosition(glm::vec3(2.0, -2.4, 1.0));
    
    glm::vec3 translation(3.0, -5.0, 3.0);
    glm::vec3 position = node->getPosition();
    glm::vec3 globalPosition = node->getPosition(Coordinates::WORLD);
    
    node->translate(translation);
    
    EXPECT_EQ(node->getPosition(Coordinates::WORLD), node->getPosition());
    EXPECT_EQ(node->getPosition(Coordinates::WORLD), globalPosition + translation);
    EXPECT_EQ(node->getPosition(), position + translation);
}

TEST_F(NodeTests, checkSetPositionLocal)
{
    std::unique_ptr<Node> node = std::make_unique<Node>();
    
    glm::vec3 position(1.0, 5.0, 3.0);
    node->setPosition(position);
    
    EXPECT_EQ(node->getPosition(), position);
    EXPECT_EQ(node->getPosition(Coordinates::WORLD), position);
}

TEST_F(NodeTests, checkSetPositionParent)
{
    std::unique_ptr<Node> root = std::make_unique<Node>();
    Node* child = new Node();
    root->addChild(std::unique_ptr<Node>(child));
    EXPECT_TRUE(root->hasChild(child));
    EXPECT_FALSE(child->hasChild(root.get()));
    
    glm::vec3 positionParent(1.0, 5.0, 3.0);
    glm::vec3 positionChild(3.0, 2.0, 1.0);
    
    root->setPosition(positionParent);
    
    EXPECT_EQ(root->getPosition(Coordinates::LOCAL), positionParent);
    EXPECT_EQ(root->getPosition(Coordinates::WORLD), positionParent);
    
    child->setPosition(positionChild);
    
    EXPECT_EQ(child->getPosition(Coordinates::LOCAL), positionChild);
    EXPECT_EQ(child->getPosition(Coordinates::WORLD), positionChild + positionParent);
    
    child->setPosition(positionChild, Coordinates::WORLD);
    
    EXPECT_EQ(child->getPosition(Coordinates::LOCAL), positionChild - positionParent);
    EXPECT_EQ(child->getPosition(Coordinates::WORLD), positionChild);

}

TEST_F(NodeTests, checkSetPositionWorld)
{
    std::unique_ptr<Node> parent = std::make_unique<Node>();
    Node* child = new Node();
    parent->addChild(std::unique_ptr<Node>(child));
    
    glm::vec3 positionParent(1.0, 5.0, 3.0);
    glm::vec3 positionChild(3.0, 2.0, 1.0);
    
    glm::vec3 globalPositionChild(1.0, 2.0, 3.0);
    
    parent->setPosition(positionParent);
    child->setPosition(positionChild);
    
    EXPECT_EQ(parent->getPosition(), positionParent);
    EXPECT_EQ(parent->getPosition(Coordinates::WORLD), positionParent);
    
    EXPECT_EQ(child->getPosition(), positionChild);
    EXPECT_EQ(child->getPosition(Coordinates::WORLD), positionParent + positionChild);
    
    child->setPosition(globalPositionChild, Coordinates::WORLD);
    
    EXPECT_EQ(child->getPosition(Coordinates::WORLD), globalPositionChild);
    
    child->translate(positionChild);
    
    EXPECT_EQ(child->getPosition(Coordinates::WORLD), globalPositionChild + positionChild);
}

TEST_F(NodeTests, checkSetScale)
{
    std::unique_ptr<Node> node = std::make_unique<Node>();
    
    glm::vec3 scale(1.0, 5.0, 3.0);
    node->setScale(scale);
    
    EXPECT_EQ(node->getScale(), scale);
}

TEST_F(NodeTests, checkSetChildPosition)
{
    std::unique_ptr<Node> parent = std::make_unique<Node>();
    Node* child = new Node();
    parent->addChild(std::unique_ptr<Node>(child));
    
    glm::vec3 positionParent(1.0, 5.0, 3.0);
    glm::vec3 positionChild(-21.0, 9.0, 8.0);
    
    parent->setPosition(positionParent);
    child->setPosition(positionChild);
    
    EXPECT_EQ(parent->getPosition(Coordinates::WORLD), parent->getPosition());
    EXPECT_EQ(parent->getPosition(Coordinates::WORLD), positionParent);
    EXPECT_EQ(parent->getPosition(), positionParent);
    
    EXPECT_EQ(child->getPosition(Coordinates::WORLD), positionParent + positionChild);
    EXPECT_EQ(child->getPosition(), positionChild);
}

TEST_F(NodeTests, checkTranslateChild)
{
    std::unique_ptr<Node> parent = std::make_unique<Node>("PARENT");
    Node* child = new Node("CHILD");
    parent->addChild(std::unique_ptr<Node>(child));
    
    parent->setPosition(glm::vec3(2.0, 4.0, -2.0));
    child->setPosition(glm::vec3(32.0, -34.0, -22.0));
    
    glm::vec3 translationParent(3.0, -5.0, 3.0);
    glm::vec3 translationChild(-4.0, 5.0, 2.0);
    
    glm::vec3 positionParentBeforeTranslation = parent->getPosition();
    glm::vec3 globalPositionParentBeforeTranslation = parent->getPosition(Coordinates::WORLD);
    
    glm::vec3 positionChildBeforeTranslation = child->getPosition();
    glm::vec3 globalPositionChildBeforeTranslation = child->getPosition(Coordinates::WORLD);
    
    parent->translate(translationParent);
    child->translate(translationChild);
    
    
    EXPECT_EQ(parent->getPosition(Coordinates::WORLD), parent->getPosition());
    EXPECT_EQ(parent->getPosition(Coordinates::WORLD), globalPositionParentBeforeTranslation + translationParent);
    EXPECT_EQ(parent->getPosition(), positionParentBeforeTranslation + translationParent);
    
    EXPECT_EQ(child->getPosition(Coordinates::WORLD), globalPositionChildBeforeTranslation + translationChild + translationParent);
    EXPECT_EQ(child->getPosition(Coordinates::WORLD), positionParentBeforeTranslation + translationParent + positionChildBeforeTranslation + translationChild);
    EXPECT_EQ(child->getPosition(Coordinates::WORLD), parent->getPosition(Coordinates::WORLD) + positionChildBeforeTranslation + translationChild);
    EXPECT_EQ(child->getPosition(), positionChildBeforeTranslation + translationChild);
}

TEST_F(NodeTests, checkSetPositionBeforeAddChild)
{
    std::unique_ptr<Node> parent = std::make_unique<Node>();
    Node* child = new Node();
    
    glm::vec3 parentPosition(1.0, 2.0, 3.0);
    glm::vec3 childPosition(1.0, 2.0, 3.0);
    
    parent->setPosition(parentPosition);
    child->setPosition(childPosition);
    
    EXPECT_EQ(parent->getPosition(Coordinates::WORLD), parentPosition);
    EXPECT_EQ(child->getPosition(Coordinates::WORLD), childPosition);
    
    parent->addChild(std::unique_ptr<Node>(child));
    
    EXPECT_EQ(child->getPosition(Coordinates::WORLD), parent->getPosition(Coordinates::WORLD) + childPosition);
    EXPECT_EQ(child->getPosition(Coordinates::WORLD), parent->getPosition(Coordinates::WORLD) + childPosition);
}

TEST_F(NodeTests, checkTranslateBeforeAddChild)
{
    std::unique_ptr<Node> parent = std::make_unique<Node>();
    Node* child = new Node();
    
    glm::vec3 parentPositionBeforeTranslation(1.0, 2.0, 3.0);
    glm::vec3 childPositionBeforeTranslation(1.0, 2.0, 3.0);
    
    glm::vec3 parentTranslate(13.0, -2.0, 5.0);
    glm::vec3 childTranslate(1.0, -32.0, 4.0);
    
    parent->setPosition(parentPositionBeforeTranslation);
    child->setPosition(childPositionBeforeTranslation);
    
    parent->translate(parentTranslate);
    child->translate(childTranslate);
    
    EXPECT_EQ(parent->getPosition(Coordinates::WORLD), parentPositionBeforeTranslation + parentTranslate);
    EXPECT_EQ(child->getPosition(Coordinates::WORLD), childPositionBeforeTranslation + childTranslate);
    
    parent->addChild(std::unique_ptr<Node>(child));
    
    EXPECT_EQ(child->getPosition(Coordinates::WORLD), parent->getPosition(Coordinates::WORLD) + child->getPosition());
    EXPECT_EQ(child->getPosition(Coordinates::WORLD), parent->getPosition(Coordinates::WORLD) + childPositionBeforeTranslation + childTranslate);
}

TEST_F(NodeTests, checkSetPositionAfterRemoveChild)
{
    std::unique_ptr<Node> parent = std::make_unique<Node>();
    Node* child = new Node();
    parent->addChild(std::unique_ptr<Node>(child));
    
    glm::vec3 parentPosition(1.0, 2.0, 3.0);
    glm::vec3 childPosition(1.0, 2.0, 3.0);
    
    parent->setPosition(parentPosition);
    child->setPosition(childPosition);
    
    EXPECT_EQ(child->getPosition(Coordinates::WORLD), parent->getPosition(Coordinates::WORLD) + childPosition);
    
    EXPECT_EQ(parent->removeChild(child)->getPosition(Coordinates::WORLD), childPosition);
    
}

TEST_F(NodeTests, checkTranslateAfterRemoveChild)
{
    std::unique_ptr<Node> parent = std::make_unique<Node>();
    Node* child = new Node();
    parent->addChild(std::unique_ptr<Node>(child));
    
    glm::vec3 parentPositionBeforeTranslation(1.0, 2.0, 3.0);
    glm::vec3 childPositionBeforeTranslation(1.0, 2.0, 3.0);
    
    glm::vec3 parentTranslate(13.0, -2.0, 5.0);
    glm::vec3 childTranslate(1.0, -32.0, 4.0);
    
    parent->setPosition(parentPositionBeforeTranslation);
    child->setPosition(childPositionBeforeTranslation);
    
    parent->translate(parentTranslate);
    child->translate(childTranslate);
    
    EXPECT_EQ(child->getPosition(Coordinates::WORLD), parent->getPosition(Coordinates::WORLD) + child->getPosition());
    EXPECT_EQ(child->getPosition(Coordinates::WORLD), parent->getPosition(Coordinates::WORLD) + childPositionBeforeTranslation + childTranslate);
    EXPECT_EQ(parent->removeChild(child)->getPosition(Coordinates::WORLD), childPositionBeforeTranslation + childTranslate);
}

TEST_F(NodeTests, checkTranslateInGraph)
{
    const unsigned int nodesCount = 50;
    
    std::unique_ptr<Node> root = std::make_unique<Node>();
    Node* currentNode = root.get();
    Node* nextNode = currentNode;
    
    glm::vec3 position(4.0, -2.0, 3.0);
    glm::vec3 translation(1.0, 2.0, 3.0);
    
    root->setPosition(position);
    root->translate(translation);
    
    root->setIdentifier("ROOT");
    
    for(unsigned int i = 0; i < nodesCount; ++i)
    {
        nextNode = new Node();
        nextNode->setIdentifier("NODE" + std::to_string(i));
        
        nextNode->setPosition(position);
        nextNode->translate(translation);
        
        currentNode->addChild(std::unique_ptr<Node>(nextNode));
        currentNode = nextNode;
    }
    
    glm::vec3 globalPosition = position + translation;
    currentNode = root.get();
    nextNode = currentNode;
    
    while (currentNode->hasChildren())
    {
        EXPECT_EQ(currentNode->getPosition(Coordinates::WORLD), globalPosition);
        
        globalPosition += (position + translation);
        currentNode = currentNode->getChildren().front().get();
    }
}

TEST_F(NodeTests, checkSetPositionIn3)
{
    std::unique_ptr<Node> root = std::make_unique<Node>();
    Node* child = new Node();
    Node* grandChild = new Node();
    Node* grandGrandChild = new Node();
    
    root->addChild(std::unique_ptr<Node>(child));
    child->addChild(std::unique_ptr<Node>(grandChild));
    grandChild->addChild(std::unique_ptr<Node>(grandGrandChild));
    
    glm::vec3 positionParent(4.0, 2.0, 3.0);
    glm::vec3 positionChild(3.0, -2.0, 3.0);
    glm::vec3 positionGrandchild(-4.0, -2.0, 3.0);
    glm::vec3 positionGrandGrandchild(4.0, -2.0, 3.0);
    
    root->setPosition(positionParent);
    child->setPosition(positionChild);
    grandChild->setPosition(positionGrandchild);
    grandGrandChild->setPosition(positionGrandGrandchild);
    
    EXPECT_EQ(grandGrandChild->getPosition(Coordinates::WORLD), grandChild->getPosition(Coordinates::WORLD) + positionGrandGrandchild);
    EXPECT_EQ(grandChild->getPosition(Coordinates::WORLD), child->getPosition(Coordinates::WORLD) + positionGrandchild);
    EXPECT_EQ(child->getPosition(Coordinates::WORLD), root->getPosition(Coordinates::WORLD) + positionChild);
    EXPECT_EQ(root->getPosition(Coordinates::WORLD), positionParent);
}

TEST_F(NodeTests, checkConnections)
{
    std::unique_ptr<Node> root = std::make_unique<Node>("ROOT");
    Node* child = new Node("CHILD");
    Node* grandChild = new Node("GRANDCHILD");
    
    root->addChild(std::unique_ptr<Node>(child));
    child->addChild(std::unique_ptr<Node>(grandChild));
    
    EXPECT_FALSE(root->hasParent());
    EXPECT_FALSE(root->isChildOf(root.get()));
    EXPECT_FALSE(root->isChildOf(child));
    EXPECT_FALSE(root->isChildOf(grandChild));
    EXPECT_TRUE(root->hasChildren());
    EXPECT_FALSE(root->hasChild(root.get()));
    EXPECT_TRUE(root->hasChild(child));
    EXPECT_FALSE(root->hasChild(grandChild));
    
    EXPECT_TRUE(child->hasParent());
    EXPECT_TRUE(child->isChildOf(root.get()));
    EXPECT_FALSE(child->isChildOf(child));
    EXPECT_FALSE(child->isChildOf(grandChild));
    EXPECT_TRUE(child->hasChildren());
    EXPECT_FALSE(child->hasChild(root.get()));
    EXPECT_FALSE(child->hasChild(child));
    EXPECT_TRUE(child->hasChild(grandChild));
    
    EXPECT_TRUE(grandChild->hasParent());
    EXPECT_FALSE(grandChild->isChildOf(root.get()));
    EXPECT_TRUE(grandChild->isChildOf(child));
    EXPECT_FALSE(grandChild->isChildOf(grandChild));
    EXPECT_FALSE(grandChild->hasChildren());
    EXPECT_FALSE(grandChild->hasChild(root.get()));
    EXPECT_FALSE(grandChild->hasChild(child));
    EXPECT_FALSE(grandChild->hasChild(grandChild));
    
    std::unique_ptr<Node> detachedChild = child->detach();
    
    EXPECT_FALSE(root->hasParent());
    EXPECT_FALSE(root->isChildOf(root.get()));
    EXPECT_FALSE(root->isChildOf(child));
    EXPECT_FALSE(root->isChildOf(grandChild));
    EXPECT_FALSE(root->hasChildren());
    EXPECT_FALSE(root->hasChild(root.get()));
    EXPECT_FALSE(root->hasChild(child));
    EXPECT_FALSE(root->hasChild(grandChild));
    
    EXPECT_FALSE(child->hasParent());
    EXPECT_FALSE(child->isChildOf(root.get()));
    EXPECT_FALSE(child->isChildOf(child));
    EXPECT_FALSE(child->isChildOf(grandChild));
    EXPECT_TRUE(child->hasChildren());
    EXPECT_FALSE(child->hasChild(root.get()));
    EXPECT_FALSE(child->hasChild(child));
    EXPECT_TRUE(child->hasChild(grandChild));
    
    EXPECT_TRUE(grandChild->hasParent());
    EXPECT_FALSE(grandChild->isChildOf(root.get()));
    EXPECT_TRUE(grandChild->isChildOf(child));
    EXPECT_FALSE(grandChild->isChildOf(grandChild));
    EXPECT_FALSE(grandChild->hasChildren());
    EXPECT_FALSE(grandChild->hasChild(root.get()));
    EXPECT_FALSE(grandChild->hasChild(child));
    EXPECT_FALSE(grandChild->hasChild(grandChild));
    
    root->addChild(grandChild->detach());
    
    EXPECT_FALSE(root->hasParent());
    EXPECT_FALSE(root->isChildOf(root.get()));
    EXPECT_FALSE(root->isChildOf(child));
    EXPECT_FALSE(root->isChildOf(grandChild));
    EXPECT_TRUE(root->hasChildren());
    EXPECT_FALSE(root->hasChild(root.get()));
    EXPECT_FALSE(root->hasChild(child));
    EXPECT_TRUE(root->hasChild(grandChild));
    
    EXPECT_FALSE(child->hasParent());
    EXPECT_FALSE(child->isChildOf(root.get()));
    EXPECT_FALSE(child->isChildOf(child));
    EXPECT_FALSE(child->isChildOf(grandChild));
    EXPECT_FALSE(child->hasChildren());
    EXPECT_FALSE(child->hasChild(root.get()));
    EXPECT_FALSE(child->hasChild(child));
    EXPECT_FALSE(child->hasChild(grandChild));
    
    EXPECT_TRUE(grandChild->hasParent());
    EXPECT_TRUE(grandChild->isChildOf(root.get()));
    EXPECT_FALSE(grandChild->isChildOf(child));
    EXPECT_FALSE(grandChild->isChildOf(grandChild));
    EXPECT_FALSE(grandChild->hasChildren());
    EXPECT_FALSE(grandChild->hasChild(root.get()));
    EXPECT_FALSE(grandChild->hasChild(child));
    EXPECT_FALSE(grandChild->hasChild(grandChild));
    
    grandChild->addChild(std::move(detachedChild));
    
    EXPECT_FALSE(root->hasParent());
    EXPECT_FALSE(root->isChildOf(root.get()));
    EXPECT_FALSE(root->isChildOf(child));
    EXPECT_FALSE(root->isChildOf(grandChild));
    EXPECT_TRUE(root->hasChildren());
    EXPECT_FALSE(root->hasChild(root.get()));
    EXPECT_FALSE(root->hasChild(child));
    EXPECT_TRUE(root->hasChild(grandChild));
    
    EXPECT_TRUE(child->hasParent());
    EXPECT_FALSE(child->isChildOf(root.get()));
    EXPECT_FALSE(child->isChildOf(child));
    EXPECT_TRUE(child->isChildOf(grandChild));
    EXPECT_TRUE(child->isChildOf("GRANDCHILD"));
    EXPECT_FALSE(child->hasChildren());
    EXPECT_FALSE(child->hasChild(root.get()));
    EXPECT_FALSE(child->hasChild(child));
    EXPECT_FALSE(child->hasChild(grandChild));
    
    EXPECT_TRUE(grandChild->hasParent());
    EXPECT_TRUE(grandChild->isChildOf(root.get()));
    EXPECT_FALSE(grandChild->isChildOf(child));
    EXPECT_FALSE(grandChild->isChildOf(grandChild));
    EXPECT_TRUE(grandChild->hasChildren());
    EXPECT_FALSE(grandChild->hasChild(root.get()));
    EXPECT_TRUE(grandChild->hasChild(child));
    EXPECT_FALSE(grandChild->hasChild(grandChild));
    
    detachedChild = grandChild->removeChild(child);
    
    EXPECT_FALSE(root->hasParent());
    EXPECT_FALSE(root->isChildOf(root.get()));
    EXPECT_FALSE(root->isChildOf(child));
    EXPECT_FALSE(root->isChildOf(grandChild));
    EXPECT_TRUE(root->hasChildren());
    EXPECT_FALSE(root->hasChild(root.get()));
    EXPECT_FALSE(root->hasChild(child));
    EXPECT_TRUE(root->hasChild(grandChild));
    
    EXPECT_FALSE(child->hasParent());
    EXPECT_FALSE(child->isChildOf(root.get()));
    EXPECT_FALSE(child->isChildOf(child));
    EXPECT_FALSE(child->isChildOf(grandChild));
    EXPECT_FALSE(child->hasChildren());
    EXPECT_FALSE(child->hasChild(root.get()));
    EXPECT_FALSE(child->hasChild(child));
    EXPECT_FALSE(child->hasChild(grandChild));
    
    EXPECT_TRUE(grandChild->hasParent());
    EXPECT_TRUE(grandChild->isChildOf(root.get()));
    EXPECT_TRUE(grandChild->isChildOf("ROOT"));
    EXPECT_FALSE(grandChild->isChildOf(child));
    EXPECT_FALSE(grandChild->isChildOf(grandChild));
    EXPECT_FALSE(grandChild->hasChildren());
    EXPECT_FALSE(grandChild->hasChild(root.get()));
    EXPECT_FALSE(grandChild->hasChild(child));
    EXPECT_FALSE(grandChild->hasChild(grandChild));
    
    (void)root->removeAllChildren();
    
    EXPECT_FALSE(root->hasParent());
    EXPECT_FALSE(root->isChildOf(root.get()));
    EXPECT_FALSE(root->isChildOf(child));
    EXPECT_FALSE(root->isChildOf(grandChild));
    EXPECT_FALSE(root->hasChildren());
    EXPECT_FALSE(root->hasChild(root.get()));
    EXPECT_FALSE(root->hasChild(child));
    EXPECT_FALSE(root->hasChild(grandChild));
    
    EXPECT_FALSE(child->hasParent());
    EXPECT_FALSE(child->isChildOf(root.get()));
    EXPECT_FALSE(child->isChildOf(child));
    EXPECT_FALSE(child->isChildOf(grandChild));
    EXPECT_FALSE(child->hasChildren());
    EXPECT_FALSE(child->hasChild(root.get()));
    EXPECT_FALSE(child->hasChild(child));
    EXPECT_FALSE(child->hasChild(grandChild));
    
    EXPECT_FALSE(grandChild->hasParent());
    EXPECT_FALSE(grandChild->isChildOf(root.get()));
    EXPECT_FALSE(grandChild->isChildOf("ROOT"));
    EXPECT_FALSE(grandChild->isChildOf(child));
    EXPECT_FALSE(grandChild->isChildOf("CHILD"));
    EXPECT_FALSE(grandChild->isChildOf(grandChild));
    EXPECT_FALSE(grandChild->isChildOf("GRANDCHILD"));
    EXPECT_FALSE(grandChild->hasChildren());
    EXPECT_FALSE(grandChild->hasChild(root.get()));
    EXPECT_FALSE(grandChild->hasChild(child));
    EXPECT_FALSE(grandChild->hasChild(grandChild));
}

TEST_F(NodeTests, checkScale)
{
    std::unique_ptr<Node> root = std::make_unique<Node>();
    
    glm::vec3 scaleVector(1.34, 2.11, 3.24);
    float scaleFactor = 0.89f;
    
    
    root->scale(scaleVector);
    root->scale(scaleFactor);
    
    EXPECT_EQ(root->getScale(), scaleVector * scaleFactor);
    
    root->setScale(scaleVector);
    EXPECT_EQ(root->getScale(), scaleVector);
}

TEST_F(NodeTests, checkTransformationsComposition)
{
    std::unique_ptr<Node> root = std::make_unique<Node>("ROOT");
    Node* child = new Node("CHILD");
    
    glm::vec3 rootPosition(0.0f, 0.0f, 0.0f);
    glm::vec3 rootRotation(0.0f, 0.0f, 45.0f);
    glm::vec3 rootScale(0.5f, 1.0f, 1.0f);
    
    glm::vec3 childPosition(-5.0f, 0.0f, 0.0f);
    glm::vec3 childRotation(0.0f, 0.0f, -45.0f);
    glm::vec3 childScale(2.0f, 1.0f, 1.0f);
    
    glm::vec3 expectedChildGlobalPosition(-1.76776671f, -1.76776671f, 0.0f);
    
    root->addChild(std::unique_ptr<Node>(child));
    
    root->setPosition(rootPosition);
    root->setRotation(glm::quat(glm::radians(rootRotation)));
    root->setScale(rootScale);
    
    child->setPosition(childPosition);
    child->setRotation(glm::quat(glm::radians(childRotation)));
    child->setScale(childScale);
    
    EXPECT_FLOAT_EQ(child->getPosition(Coordinates::WORLD).x, expectedChildGlobalPosition.x);
    EXPECT_FLOAT_EQ(child->getPosition(Coordinates::WORLD).y, expectedChildGlobalPosition.y);
    EXPECT_FLOAT_EQ(child->getPosition(Coordinates::WORLD).z, expectedChildGlobalPosition.z);
}

TEST_F(NodeTests, checkIdentifier)
{
    std::string rootIdentifier = "ROOT";
    std::unique_ptr<Node> root = std::make_unique<Node>(rootIdentifier);

    std::string childIdentifier = "CHILDREN";
    root->addChild(std::unique_ptr<Node>(new Node(childIdentifier)));

    EXPECT_EQ(root->getIdentifier(), rootIdentifier);
    EXPECT_TRUE(root->hasChild(childIdentifier));

    (void)root->removeChild(childIdentifier);
    EXPECT_FALSE(root->hasChild(childIdentifier));

}

TEST_F(NodeTests, checkHasChildWithNull)
{
    std::unique_ptr<Node> node = std::make_unique<Node>("NODE");
    EXPECT_FALSE(node->hasChild(nullptr));
}

TEST_F(NodeTests, checkWorldRotation)
{
    std::unique_ptr<Node> parent = std::make_unique<Node>("PARENT");
    Node* child = new Node("CHILD");
    parent->addChild(std::unique_ptr<Node>(child));

    glm::quat parentRot = glm::angleAxis(glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    glm::quat childRot = glm::angleAxis(glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    parent->setRotation(parentRot);
    child->setRotation(childRot);

    glm::quat expectedWorld = parentRot * childRot;
    glm::quat actualWorld = child->getRotation(Coordinates::WORLD);

    // Compare quaternions (account for sign ambiguity: q and -q represent same rotation)
    float dot = glm::dot(expectedWorld, actualWorld);
    EXPECT_GT(std::abs(dot), 0.9999f);
}

TEST_F(NodeTests, checkWorldTranslateWithParentScale)
{
    std::unique_ptr<Node> parent = std::make_unique<Node>("PARENT");
    Node* child = new Node("CHILD");
    parent->addChild(std::unique_ptr<Node>(child));

    parent->setScale(glm::vec3(2.0f));
    child->setPosition(glm::vec3(0.0f));
    child->translate(glm::vec3(2.0f, 0.0f, 0.0f), Coordinates::WORLD);

    glm::vec3 worldPos = child->getPosition(Coordinates::WORLD);
    EXPECT_FLOAT_EQ(worldPos.x, 2.0f);
    EXPECT_FLOAT_EQ(worldPos.y, 0.0f);
    EXPECT_FLOAT_EQ(worldPos.z, 0.0f);

    glm::vec3 localPos = child->getPosition(Coordinates::LOCAL);
    EXPECT_FLOAT_EQ(localPos.x, 1.0f);
    EXPECT_FLOAT_EQ(localPos.y, 0.0f);
    EXPECT_FLOAT_EQ(localPos.z, 0.0f);
}

TEST_F(NodeTests, checkTranslateParentSpace)
{
    std::unique_ptr<Node> parent = std::make_unique<Node>("PARENT");
    Node* child = new Node("CHILD");
    parent->addChild(std::unique_ptr<Node>(child));

    // Rotate child 90 degrees around Z
    child->setRotation(glm::angleAxis(glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f)));

    // Translate in PARENT space (should move along parent's X, not child's rotated X)
    child->translate(glm::vec3(1.0f, 0.0f, 0.0f), Coordinates::PARENT);

    glm::vec3 pos = child->getPosition(Coordinates::PARENT);
    EXPECT_FLOAT_EQ(pos.x, 1.0f);
    EXPECT_FLOAT_EQ(pos.y, 0.0f);
    EXPECT_FLOAT_EQ(pos.z, 0.0f);

    // Compare with LOCAL translation which applies rotation
    child->setPosition(glm::vec3(0.0f));
    child->translate(glm::vec3(1.0f, 0.0f, 0.0f), Coordinates::LOCAL);

    glm::vec3 localPos = child->getPosition(Coordinates::LOCAL);
    // LOCAL applies 90-degree Z rotation, so X becomes Y
    EXPECT_NEAR(localPos.x, 0.0f, 1e-6f);
    EXPECT_NEAR(localPos.y, 1.0f, 1e-6f);
    EXPECT_NEAR(localPos.z, 0.0f, 1e-6f);
}

TEST_F(NodeTests, checkRotateParentSpace)
{
    std::unique_ptr<Node> parent = std::make_unique<Node>("PARENT");
    Node* child = new Node("CHILD");
    parent->addChild(std::unique_ptr<Node>(child));

    // Rotate child 90 degrees around Z in local space
    child->setRotation(glm::angleAxis(glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f)));

    // Rotate 90 degrees around parent's Y axis
    child->rotate(glm::vec3(0.0f, 1.0f, 0.0f), glm::radians(90.0f), Coordinates::PARENT);

    // The rotation should be applied relative to parent's axes
    glm::quat rot = child->getRotation(Coordinates::PARENT);
    // Verify rotation was applied correctly (not identity)
    EXPECT_FALSE(rot == glm::identity<glm::quat>());
}

TEST_F(NodeTests, checkGetSetPositionParentSpace)
{
    std::unique_ptr<Node> parent = std::make_unique<Node>("PARENT");
    Node* child = new Node("CHILD");
    parent->addChild(std::unique_ptr<Node>(child));

    glm::vec3 position(5.0f, 3.0f, 2.0f);
    child->setPosition(position, Coordinates::PARENT);

    EXPECT_EQ(child->getPosition(Coordinates::PARENT), position);
    EXPECT_EQ(child->getPosition(Coordinates::LOCAL), position);
}

// === Missing Method Tests ===

TEST_F(NodeTests, checkGetMatrix)
{
    std::unique_ptr<Node> node = std::make_unique<Node>();

    glm::vec3 position(1.0f, 2.0f, 3.0f);
    glm::vec3 scale(2.0f, 2.0f, 2.0f);
    glm::quat rotation = glm::angleAxis(glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    node->setPosition(position);
    node->setScale(scale);
    node->setRotation(rotation);

    const glm::mat4& matrix = node->getMatrix();

    glm::vec4 origin = matrix * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    EXPECT_VEC3_NEAR(glm::vec3(origin), position);
}

TEST_F(NodeTests, checkGetEulerRotationRoundTrip)
{
    std::unique_ptr<Node> node = std::make_unique<Node>();

    glm::vec3 eulerAngles(glm::radians(30.0f), glm::radians(45.0f), glm::radians(60.0f));
    node->setRotation(eulerAngles);

    glm::vec3 retrieved = node->getEulerRotation();

    glm::quat original = glm::quat(eulerAngles);
    glm::quat reconstructed = glm::quat(retrieved);

    EXPECT_QUAT_NEAR(original, reconstructed);
}

TEST_F(NodeTests, checkAttachTo)
{
    std::unique_ptr<Node> parent = std::make_unique<Node>("PARENT");
    std::unique_ptr<Node> child = std::make_unique<Node>("CHILD");
    Node* childPtr = child.get();

    Node::attachTo(std::move(child), parent.get());

    EXPECT_TRUE(parent->hasChild(childPtr));
    EXPECT_TRUE(childPtr->isChildOf(parent.get()));
    EXPECT_EQ(childPtr->getParent(), parent.get());
}

TEST_F(NodeTests, checkSetScaleIndividualComponents)
{
    std::unique_ptr<Node> node = std::make_unique<Node>();

    node->setScale(1.5f, 2.5f, 3.5f);

    EXPECT_FLOAT_EQ(node->getScale().x, 1.5f);
    EXPECT_FLOAT_EQ(node->getScale().y, 2.5f);
    EXPECT_FLOAT_EQ(node->getScale().z, 3.5f);
}

TEST_F(NodeTests, checkSetRotationAxisAngle)
{
    std::unique_ptr<Node> node = std::make_unique<Node>();

    glm::vec3 axis(0.0f, 1.0f, 0.0f);
    float angle = glm::radians(90.0f);

    node->setRotation(axis, angle);

    glm::quat expected = glm::angleAxis(angle, axis);
    EXPECT_QUAT_NEAR(node->getRotation(), expected);
}

TEST_F(NodeTests, checkUniformScale)
{
    std::unique_ptr<Node> node = std::make_unique<Node>();

    node->setScale(3.0f);

    EXPECT_FLOAT_EQ(node->getScale().x, 3.0f);
    EXPECT_FLOAT_EQ(node->getScale().y, 3.0f);
    EXPECT_FLOAT_EQ(node->getScale().z, 3.0f);
}

// === Edge Case Tests ===

TEST_F(NodeTests, checkExtremePositionValues)
{
    std::unique_ptr<Node> node = std::make_unique<Node>();

    glm::vec3 extremePos(1e10f, -1e10f, 1e10f);
    node->setPosition(extremePos);

    EXPECT_VEC3_NEAR(node->getPosition(), extremePos, 1e5f);
}

TEST_F(NodeTests, checkZeroScale)
{
    std::unique_ptr<Node> node = std::make_unique<Node>();

    node->setScale(glm::vec3(0.0f));

    EXPECT_FLOAT_EQ(node->getScale().x, 0.0f);
    EXPECT_FLOAT_EQ(node->getScale().y, 0.0f);
    EXPECT_FLOAT_EQ(node->getScale().z, 0.0f);

    const glm::mat4& matrix = node->getMatrix();
    EXPECT_TRUE(std::isfinite(matrix[0][0]));
}

TEST_F(NodeTests, checkNegativeScale)
{
    std::unique_ptr<Node> node = std::make_unique<Node>();

    node->setScale(glm::vec3(-1.0f, -2.0f, -3.0f));

    EXPECT_FLOAT_EQ(node->getScale().x, -1.0f);
    EXPECT_FLOAT_EQ(node->getScale().y, -2.0f);
    EXPECT_FLOAT_EQ(node->getScale().z, -3.0f);
}

TEST_F(NodeTests, checkRemoveNonExistentChild)
{
    std::unique_ptr<Node> parent = std::make_unique<Node>("PARENT");
    std::unique_ptr<Node> unrelated = std::make_unique<Node>("UNRELATED");

    auto result = parent->removeChild(unrelated.get());

    EXPECT_EQ(result, nullptr);
}

TEST_F(NodeTests, checkVeryDeepHierarchy)
{
    const unsigned int depth = 1000;

    std::unique_ptr<Node> root = std::make_unique<Node>("ROOT");
    root->setPosition(glm::vec3(1.0f, 0.0f, 0.0f));

    Node* current = root.get();
    for (unsigned int i = 0; i < depth; ++i)
    {
        auto child = std::make_unique<Node>("NODE_" + std::to_string(i));
        child->setPosition(glm::vec3(1.0f, 0.0f, 0.0f));
        Node* childPtr = child.get();
        current->addChild(std::move(child));
        current = childPtr;
    }

    glm::vec3 worldPos = current->getPosition(Coordinates::WORLD);
    EXPECT_NEAR(worldPos.x, static_cast<float>(depth + 1), 0.1f);
}

TEST_F(NodeTests, checkMultipleChildren)
{
    std::unique_ptr<Node> parent = std::make_unique<Node>("PARENT");

    for (int i = 0; i < 10; ++i)
    {
        auto child = std::make_unique<Node>("CHILD_" + std::to_string(i));
        parent->addChild(std::move(child));
    }

    EXPECT_EQ(parent->getChildren().size(), 10u);

    auto allChildren = parent->removeAllChildren();
    EXPECT_EQ(allChildren.size(), 10u);
    EXPECT_FALSE(parent->hasChildren());
}

TEST_F(NodeTests, checkIdentityTransformations)
{
    std::unique_ptr<Node> node = std::make_unique<Node>();

    const glm::mat4& matrix = node->getMatrix();

    EXPECT_FLOAT_EQ(matrix[0][0], 1.0f);
    EXPECT_FLOAT_EQ(matrix[1][1], 1.0f);
    EXPECT_FLOAT_EQ(matrix[2][2], 1.0f);
    EXPECT_FLOAT_EQ(matrix[3][3], 1.0f);
}

TEST_F(NodeTests, checkMatrixCaching)
{
    std::unique_ptr<Node> node = std::make_unique<Node>();

    node->setPosition(glm::vec3(1.0f, 2.0f, 3.0f));

    const glm::mat4& matrix1 = node->getMatrix();
    const glm::mat4& matrix2 = node->getMatrix();

    EXPECT_EQ(&matrix1, &matrix2);
}

TEST_F(NodeTests, checkGlobalMatrixCaching)
{
    std::unique_ptr<Node> parent = std::make_unique<Node>("PARENT");
    Node* child = new Node("CHILD");
    parent->addChild(std::unique_ptr<Node>(child));

    const glm::mat4& global1 = child->getGlobalMatrix();
    const glm::mat4& global2 = child->getGlobalMatrix();

    EXPECT_EQ(&global1, &global2);
}

TEST_F(NodeTests, checkEmptyIdentifier)
{
    std::unique_ptr<Node> node = std::make_unique<Node>();

    EXPECT_TRUE(node->getIdentifier().empty());

    node->setIdentifier("TEST");
    EXPECT_EQ(node->getIdentifier(), "TEST");

    node->setIdentifier("");
    EXPECT_TRUE(node->getIdentifier().empty());
}

TEST_F(NodeTests, checkRotateWithQuaternion)
{
    std::unique_ptr<Node> node = std::make_unique<Node>();

    glm::quat rotation = glm::angleAxis(glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    node->rotate(rotation);

    EXPECT_QUAT_NEAR(node->getRotation(), rotation);
}

TEST_F(NodeTests, checkRotateWithEulerFloats)
{
    std::unique_ptr<Node> node = std::make_unique<Node>();

    float xAngle = glm::radians(30.0f);
    float yAngle = glm::radians(45.0f);
    float zAngle = glm::radians(60.0f);

    node->rotate(xAngle, yAngle, zAngle);

    glm::quat rot = node->getRotation();
    EXPECT_FALSE(rot == glm::identity<glm::quat>());
}

TEST_F(NodeTests, checkScaleMultiplication)
{
    std::unique_ptr<Node> node = std::make_unique<Node>();

    node->setScale(glm::vec3(2.0f));
    node->scale(3.0f);

    EXPECT_VEC3_NEAR(node->getScale(), glm::vec3(6.0f));
}

TEST_F(NodeTests, checkScaleWithComponents)
{
    std::unique_ptr<Node> node = std::make_unique<Node>();

    node->setScale(glm::vec3(2.0f));
    node->scale(1.0f, 2.0f, 3.0f);

    EXPECT_FLOAT_EQ(node->getScale().x, 2.0f);
    EXPECT_FLOAT_EQ(node->getScale().y, 4.0f);
    EXPECT_FLOAT_EQ(node->getScale().z, 6.0f);
}

// === Hierarchy Traversal Tests ===

TEST_F(NodeTests, checkFindByIdentifier)
{
    std::unique_ptr<Node> root = std::make_unique<Node>("ROOT");
    auto child1 = std::make_unique<Node>("CHILD1");
    auto child2 = std::make_unique<Node>("CHILD2");
    auto grandChild = std::make_unique<Node>("GRANDCHILD");

    Node* grandChildPtr = grandChild.get();
    child1->addChild(std::move(grandChild));
    root->addChild(std::move(child1));
    root->addChild(std::move(child2));

    EXPECT_EQ(root->findByIdentifier("GRANDCHILD"), grandChildPtr);
    EXPECT_EQ(root->findByIdentifier("CHILD2"), root->getChildren().back().get());
    EXPECT_EQ(root->findByIdentifier("NONEXISTENT"), nullptr);
}

TEST_F(NodeTests, checkGetRoot)
{
    std::unique_ptr<Node> root = std::make_unique<Node>("ROOT");
    auto child = std::make_unique<Node>("CHILD");
    auto grandChild = std::make_unique<Node>("GRANDCHILD");

    Node* childPtr = child.get();
    Node* grandChildPtr = grandChild.get();

    child->addChild(std::move(grandChild));
    root->addChild(std::move(child));

    EXPECT_EQ(root->getRoot(), root.get());
    EXPECT_EQ(childPtr->getRoot(), root.get());
    EXPECT_EQ(grandChildPtr->getRoot(), root.get());
}

TEST_F(NodeTests, checkGetDepth)
{
    std::unique_ptr<Node> root = std::make_unique<Node>("ROOT");
    auto child = std::make_unique<Node>("CHILD");
    auto grandChild = std::make_unique<Node>("GRANDCHILD");

    Node* childPtr = child.get();
    Node* grandChildPtr = grandChild.get();

    child->addChild(std::move(grandChild));
    root->addChild(std::move(child));

    EXPECT_EQ(root->getDepth(), 0u);
    EXPECT_EQ(childPtr->getDepth(), 1u);
    EXPECT_EQ(grandChildPtr->getDepth(), 2u);
}

TEST_F(NodeTests, checkGetSiblings)
{
    std::unique_ptr<Node> root = std::make_unique<Node>("ROOT");
    auto child1 = std::make_unique<Node>("CHILD1");
    auto child2 = std::make_unique<Node>("CHILD2");
    auto child3 = std::make_unique<Node>("CHILD3");

    Node* child1Ptr = child1.get();
    Node* child2Ptr = child2.get();
    Node* child3Ptr = child3.get();

    root->addChild(std::move(child1));
    root->addChild(std::move(child2));
    root->addChild(std::move(child3));

    EXPECT_EQ(child1Ptr->getPreviousSibling(), nullptr);
    EXPECT_EQ(child1Ptr->getNextSibling(), child2Ptr);

    EXPECT_EQ(child2Ptr->getPreviousSibling(), child1Ptr);
    EXPECT_EQ(child2Ptr->getNextSibling(), child3Ptr);

    EXPECT_EQ(child3Ptr->getPreviousSibling(), child2Ptr);
    EXPECT_EQ(child3Ptr->getNextSibling(), nullptr);
}

TEST_F(NodeTests, checkTraverse)
{
    std::unique_ptr<Node> root = std::make_unique<Node>("ROOT");
    auto child1 = std::make_unique<Node>("CHILD1");
    auto child2 = std::make_unique<Node>("CHILD2");
    auto grandChild = std::make_unique<Node>("GRANDCHILD");

    child1->addChild(std::move(grandChild));
    root->addChild(std::move(child1));
    root->addChild(std::move(child2));

    std::vector<std::string> visited;
    root->traverse([&visited](Node& node) {
        visited.push_back(std::string(node.getIdentifier()));
    });

    EXPECT_EQ(visited.size(), 4u);
    EXPECT_EQ(visited[0], "ROOT");
    EXPECT_EQ(visited[1], "CHILD1");
    EXPECT_EQ(visited[2], "GRANDCHILD");
    EXPECT_EQ(visited[3], "CHILD2");
}

// === Direction Vector Tests ===

TEST_F(NodeTests, checkDirectionVectorsIdentity)
{
    std::unique_ptr<Node> node = std::make_unique<Node>();

    glm::vec3 forward = node->getForward();
    glm::vec3 right = node->getRight();
    glm::vec3 up = node->getUp();

    EXPECT_VEC3_NEAR(forward, glm::vec3(0.0f, 0.0f, -1.0f));
    EXPECT_VEC3_NEAR(right, glm::vec3(1.0f, 0.0f, 0.0f));
    EXPECT_VEC3_NEAR(up, glm::vec3(0.0f, 1.0f, 0.0f));
}

TEST_F(NodeTests, checkDirectionVectorsRotated)
{
    std::unique_ptr<Node> node = std::make_unique<Node>();

    node->setRotation(glm::angleAxis(glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)));

    glm::vec3 forward = node->getForward();
    glm::vec3 right = node->getRight();

    EXPECT_VEC3_NEAR(forward, glm::vec3(-1.0f, 0.0f, 0.0f));
    EXPECT_VEC3_NEAR(right, glm::vec3(0.0f, 0.0f, -1.0f));
}

TEST_F(NodeTests, checkLookAt)
{
    std::unique_ptr<Node> node = std::make_unique<Node>();

    node->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
    node->lookAt(glm::vec3(0.0f, 0.0f, -10.0f));

    glm::vec3 forward = node->getForward();
    EXPECT_VEC3_NEAR(forward, glm::vec3(0.0f, 0.0f, -1.0f));
}

TEST_F(NodeTests, checkLookAtOffAxis)
{
    std::unique_ptr<Node> node = std::make_unique<Node>();

    node->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
    node->lookAt(glm::vec3(10.0f, 0.0f, 0.0f));

    glm::vec3 forward = node->getForward();
    EXPECT_VEC3_NEAR(forward, glm::vec3(1.0f, 0.0f, 0.0f));
}

// === Clone Tests ===

TEST_F(NodeTests, checkCloneSimple)
{
    std::unique_ptr<Node> original = std::make_unique<Node>("ORIGINAL");
    original->setPosition(glm::vec3(1.0f, 2.0f, 3.0f));
    original->setRotation(glm::angleAxis(glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
    original->setScale(glm::vec3(2.0f));

    auto cloned = original->clone();

    EXPECT_EQ(cloned->getIdentifier(), "ORIGINAL");
    EXPECT_VEC3_NEAR(cloned->getPosition(), original->getPosition());
    EXPECT_QUAT_NEAR(cloned->getRotation(), original->getRotation());
    EXPECT_VEC3_NEAR(cloned->getScale(), original->getScale());

    EXPECT_NE(cloned.get(), original.get());
}

TEST_F(NodeTests, checkCloneDeep)
{
    std::unique_ptr<Node> root = std::make_unique<Node>("ROOT");
    auto child = std::make_unique<Node>("CHILD");
    auto grandChild = std::make_unique<Node>("GRANDCHILD");

    grandChild->setPosition(glm::vec3(5.0f, 5.0f, 5.0f));
    child->addChild(std::move(grandChild));
    root->addChild(std::move(child));

    auto cloned = root->clone();

    EXPECT_TRUE(cloned->hasChildren());
    Node* clonedChild = cloned->findByIdentifier("CHILD");
    EXPECT_NE(clonedChild, nullptr);

    Node* clonedGrandChild = cloned->findByIdentifier("GRANDCHILD");
    EXPECT_NE(clonedGrandChild, nullptr);
    EXPECT_VEC3_NEAR(clonedGrandChild->getPosition(), glm::vec3(5.0f, 5.0f, 5.0f));
}

TEST_F(NodeTests, checkCloneIndependence)
{
    std::unique_ptr<Node> original = std::make_unique<Node>("ORIGINAL");
    original->setPosition(glm::vec3(1.0f, 2.0f, 3.0f));

    auto cloned = original->clone();

    original->setPosition(glm::vec3(10.0f, 20.0f, 30.0f));

    EXPECT_VEC3_NEAR(cloned->getPosition(), glm::vec3(1.0f, 2.0f, 3.0f));
    EXPECT_VEC3_NEAR(original->getPosition(), glm::vec3(10.0f, 20.0f, 30.0f));
}
