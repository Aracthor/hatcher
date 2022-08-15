#include "Pathfinding.hpp"

#include <set>

#include "hatcher/ComponentManager.hpp"
#include "hatcher/Maths/Box.hpp"

#include "Obstacle2DComponent.hpp"

using namespace hatcher;

namespace Pathfinding
{
namespace
{

bool Intersects(const glm::vec2& a1, const glm::vec2& a2, const glm::vec2& b1, const glm::vec2& b2)
{
    const glm::vec2 a = a2 - a1;
    const glm::vec2 b = b2 - b1;
    const float dot = glm::dot(a, b);

    if (dot == 0) // Parallel lines
        return false;

    const glm::vec2 c = b1 - a1;
    const float t = glm::dot(c, b) / dot;
    if (t < 0 || t > 1)
        return false;

    const float u = (c.x * a.y - c.y * a.x) / dot;
    if (u < 0 || u > 1)
        return false;

    return true;
}

bool Intersects(const Box2f& box, const glm::vec2& a, const glm::vec2& b)
{
    if ((a.x <= box.Min().x && b.x <= box.Min().x) || (a.y <= box.Min().y && b.y <= box.Min().y) ||
        (a.x >= box.Max().x && b.x >= box.Max().x) || (a.y >= box.Max().y && b.y >= box.Max().y))
        return false;

    glm::vec2 corners[4] = {
        {box.Min().x, box.Min().y},
        {box.Max().x, box.Min().y},
        {box.Max().x, box.Max().y},
        {box.Min().x, box.Max().y},
    };

    for (int i = 0; i < 4; i++)
    {
        int nextCorner = (i == 3) ? 0 : (i + 1);
        if (Intersects(a, b, corners[i], corners[nextCorner]))
            return true;
    }

    return false;
}

struct Graph
{
    struct Node
    {
        glm::vec2 pos;
        std::vector<int> links;
    };
    std::vector<Node> nodes;

    struct NodeValueSorter
    {
        const Graph& m_graph;
        const std::vector<float>& m_costs;
        const glm::vec2 m_endPos;

        int GetNodeValue(int node) const
        {
            return m_costs[node] + glm::length(m_graph.nodes[node].pos - m_endPos);
        }

        bool operator()(int nodeA, int nodeB) const
        {
            const int nodeAValue = GetNodeValue(nodeA);
            const int nodeBValue = GetNodeValue(nodeB);
            return nodeAValue < nodeBValue;
        };
    };
    using NodeSet = std::multiset<int, NodeValueSorter>;

    std::vector<glm::vec2> Resolve(int start, int end)
    {
        const glm::vec2& objective = nodes[end].pos;
        const int size = nodes.size();
        std::vector<float> costs;
        std::vector<int> previous;
        costs.resize(size);
        previous.resize(size, -1);
        previous[start] = 0;

        const NodeValueSorter sorter = {*this, costs, objective};
        NodeSet toVisit(sorter);
        toVisit.insert(start);
        costs[start] = 0;
        while (!toVisit.empty() && *toVisit.begin() != end)
        {
            const int nodeIndex = *toVisit.begin();
            toVisit.erase(toVisit.begin());
            const Node& node = nodes[nodeIndex];
            for (int link : node.links)
            {
                if (previous[link] == -1)
                {
                    previous[link] = nodeIndex;
                    costs[link] = costs[nodeIndex] + glm::length(node.pos - nodes[link].pos);
                    toVisit.insert(link);
                }
            }
        }

        if (!toVisit.empty() && *toVisit.begin() == end)
        {
            std::vector<glm::vec2> result;
            int nodeIndex = end;
            while (nodeIndex != start)
            {
                result.push_back(nodes[nodeIndex].pos);
                nodeIndex = previous[nodeIndex];
            }
            return result;
        }

        return {};
    }
};

bool CanBeLinked(const Graph::Node& nodeA, const Graph::Node& nodeB,
                 const std::vector<Box2f>& boxes)
{
    for (const Box2f& box : boxes)
    {
        if (Intersects(box, nodeA.pos, nodeB.pos))
            return false;
    }
    return true;
}

std::vector<Box2f> GetObstacleBoxes(const ComponentManager* componentManager, float borderOffset)
{
    std::vector<Box2f> boxes;

    auto obstacles = componentManager->GetComponents<Obstacle2DComponent>();
    for (std::optional<const Obstacle2DComponent> obstacle : obstacles)
    {
        if (obstacle)
        {
            Box2f obstacleBox = Box2f(obstacle->corners.begin(), obstacle->corners.end());
            obstacleBox.SetMin(obstacleBox.Min() - glm::vec2(borderOffset));
            obstacleBox.SetMax(obstacleBox.Max() + glm::vec2(borderOffset));
            boxes.push_back(obstacleBox);
        }
    }

    return boxes;
}

Graph CreateGraph(const glm::vec2& start, const glm::vec2& end,
                  const ComponentManager* componentManager, float borderOffset)
{
    std::vector<Box2f> boxes = GetObstacleBoxes(componentManager, borderOffset);

    Graph graph;
    graph.nodes.push_back({start});
    graph.nodes.push_back({end});
    for (const Box2f& box : boxes)
    {
        auto corners = box.GetCorners();
        for (const glm::vec2& corner : corners)
            graph.nodes.push_back({corner});
    }

    for (Graph::Node& node : graph.nodes)
    {
        for (int i = 0; i < (int)graph.nodes.size(); i++)
        {
            if (node.pos != graph.nodes[i].pos && CanBeLinked(node, graph.nodes[i], boxes))
                node.links.push_back(i);
        }
    }

    return graph;
}

} // namespace

std::vector<glm::vec2> GetPath(const glm::vec2& start, const glm::vec2& end,
                               const ComponentManager* componentManager, float borderOffset)
{
    Graph graph = CreateGraph(start, end, componentManager, borderOffset);
    return graph.Resolve(0, 1);
}

} // namespace Pathfinding
