#pragma once

#include <memory>
#include <vector>

#include "hatcher/glm_pure.hpp"

class Pathfinding
{
public:
    void CreateNode(glm::vec2 position);
    void LinkNodes(glm::vec2 positionA, glm::vec2 positionB);
    void DeleteNode(glm::vec2 position);

    std::vector<glm::vec2> GetPath(glm::vec2 startPos, glm::vec2 endPos) const;

    struct Node
    {
        glm::vec2 pos;
        std::vector<std::shared_ptr<Node>> links;
    };

private:
    std::shared_ptr<Node> FindNodeByPosition(glm::vec2 position);
    const std::shared_ptr<Node> FindNodeByPosition(glm::vec2 position) const;

    std::vector<std::shared_ptr<Node>> m_nodes;
};
