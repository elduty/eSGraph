//
//  NodeTests.cpp
//  eSGraph
//
//  Created by elduty on 03/08/2019.
//

#include "NodeTests.hpp"
#include "Node.hpp"
#include <memory>
#include <gtest/gtest.h>

using namespace eSGraph;

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
    
    (void)grandChild->removeChild(child);
    
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
