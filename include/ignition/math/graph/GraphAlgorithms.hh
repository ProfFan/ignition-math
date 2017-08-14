/*
 * Copyright (C) 2017 Open Source Robotics Foundation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
*/
#ifndef IGNITION_MATH_GRAPH_GRAPHALGORITHMS_HH_
#define IGNITION_MATH_GRAPH_GRAPHALGORITHMS_HH_

#include <algorithm>
#include <functional>
#include <list>
#include <map>
#include <queue>
#include <stack>
#include <utility>
#include <vector>

#include "ignition/math/graph/Graph.hh"
#include "ignition/math/Helpers.hh"

namespace ignition
{
namespace math
{
namespace graph
{
  /// \def CostInfo.
  /// \brief Used in Dijkstra. For a given source vertex, this pair represents
  /// the cost (first element) to reach a destination vertex (second element).
  using CostInfo = std::pair<double, VertexId>;

  /// \brief Breadth first sort (BFS).
  /// Starting from the vertex == _from, it traverses the graph exploring the
  /// neighbors first, before moving to the next level neighbors.
  /// \param[in] _graph A graph.
  /// \param[in] _from The starting vertex.
  /// \return The vector of vertices Ids traversed in a breadth first manner.
  template<typename V, typename E, typename EdgeType>
  std::vector<VertexId> BreadthFirstSort(const Graph<V, E, EdgeType> &_graph,
                                         const VertexId &_from)
  {
    // Create an auxiliary graph, where the data is just a boolean value that
    // stores whether the vertex has been visited or not.
    Graph<bool, E, EdgeType> visitorGraph;

    // Copy the vertices (just the Id).
    for (auto const &v : _graph.Vertices())
      visitorGraph.AddVertex("", false, v.first);

    // Copy the edges (without data).
    for (auto const &e : _graph.Edges())
      visitorGraph.AddEdge(e.second.get().Vertices(), E());

    std::vector<VertexId> visited;
    std::list<VertexId> pending = {_from};

    while (!pending.empty())
    {
      auto vId = pending.front();
      pending.pop_front();

      // The vertex has been visited yet.
      if (std::find(visited.begin(), visited.end(), vId) != visited.end())
        continue;

      visited.push_back(vId);
      auto &vertex = visitorGraph.VertexFromId(vId);
      vertex.Data() = true;

      // Add more vertices to visit if they haven't been visited yet.
      auto adjacents = visitorGraph.AdjacentsFrom(vId);
      for (auto const &adj : adjacents)
      {
        vId = adj.first;
        auto &vertex = adj.second.get();
        if (!vertex.Data())
          pending.push_back(vId);
      }
    }

    return visited;
  }

  /// \brief Depth first sort (DFS).
  /// Starting from the vertex == _root, it visits the graph as far as
  /// possible along each branch before backtracking.
  /// \param[in] _graph A graph.
  /// \param[in] _root The starting vertex.
  /// \return The vector of vertices Ids visited in a depth first manner.
  template<typename V, typename E, typename EdgeType>
  std::vector<VertexId> DepthFirstSort(const Graph<V, E, EdgeType> &_graph,
                                       const VertexId &_root)
  {
    std::vector<VertexId> visited;
    std::stack<VertexId> pending({_root});

    while (!pending.empty())
    {
      auto v = pending.top();
      pending.pop();

      // The vertex hasn't been visited yet.
      if (std::find(visited.begin(), visited.end(), v) != visited.end())
        continue;

      visited.push_back(v);

      // Add more vertices to visit if they haven't been visited yet.
      auto adjacents = _graph.AdjacentsFrom(v);
      for (auto const &adj : adjacents)
      {
        v = adj.first;
        if (std::find(visited.begin(), visited.end(), v) == visited.end())
          pending.push(v);
      }
    }

    return visited;
  }

  /// \brief Dijkstra algorithm.
  /// Find the shortest path between the vertices in a graph.
  /// If only a graph and a source vertex is provided, the algorithm will
  /// find shortest paths from the source vertex to all other vertices in the
  /// graph. If an additional destination vertex is provided, the algorithm
  /// will stop when the shortest path is found between the source and
  /// destination vertex.
  /// \param[in] _graph A graph.
  /// \param[in] _from The starting vertex.
  /// \param[in] _to Optional destination vertex.
  /// \return A map where the keys are the destination vertices. For each
  /// destination, the value is another pair, where the key is the shortest
  /// cost from the origin vertex. The value is the previous neighbor Id in the
  /// shortest path.
  /// Note: In the case of providing a destination vertex, only the entry in the
  /// map with key = _to should be used. The rest of the map may contain
  /// incomplete information. If you want all shortest paths to all other
  /// vertices, please remove the destination vertex.
  /// If the source or destination vertex don't exist, the function will return
  /// an empty map.
  ///
  /// E.g.: Given the following undirected graph, g, with five vertices:
  ///
  ///              (6)
  ///           0-------1
  ///           |      /|\
  ///           |     / | \(5)
  ///           | (2)/  |  \
  ///           |   /   |   2
  ///        (1)|  / (2)|  /
  ///           | /     | /(5)
  ///           |/      |/
  ///           3-------4
  ///              (1)
  ///
  /// This is the resut of Dijkstra(g, 0):
  ///
  /// ================================
  /// | Dst | Cost | Previous vertex |
  /// ================================
  /// |  0  |  0   |        0        |
  /// |  1  |  3   |        3        |
  /// |  2  |  7   |        4        |
  /// |  3  |  1   |        0        |
  /// |  4  |  2   |        3        |
  /// ================================
  ///
  /// This is the result of Dijkstra(g, 0, 3):
  ///
  /// ================================
  /// | Dst | Cost | Previous vertex |
  /// ================================
  /// |  0  |  0   |        0        |
  /// |  1  |ignore|     ignore      |
  /// |  2  |ignore|     ignore      |
  /// |  3  |  1   |        0        |
  /// |  4  |ignore|     ignore      |
  /// ================================
  template<typename V, typename E, typename EdgeType>
  std::map<VertexId, CostInfo> Dijkstra(const Graph<V, E, EdgeType> &_graph,
                                        const VertexId &_from,
                                        const VertexId &_to = kNullId)
  {
    auto allVertices = _graph.Vertices();

    // Sanity check: The source vertex should exist.
    if (allVertices.find(_from) == allVertices.end())
    {
      std::cerr << "Vertex [" << _from << "] Not found" << std::endl;
      return {};
    }

    // Sanity check: The destination vertex should exist (if used).
    if (_to != kNullId &&
        allVertices.find(_to) == allVertices.end())
    {
      std::cerr << "Vertex [" << _from << "] Not found" << std::endl;
      return {};
    }

    // Store vertices that are being preprocessed.
    std::priority_queue<CostInfo,
      std::vector<CostInfo>, std::greater<CostInfo>> pq;

    // Create a map for distances and next neightbor and initialize all
    // distances as infinite.
    std::map<VertexId, CostInfo> dist;
    for (auto const &v : allVertices)
    {
      auto id = v.first;
      dist[id] = std::make_pair(MAX_D, kNullId);
    }

    // Insert _from in the priority queue and initialize its distance as 0.
    pq.push(std::make_pair(0.0, _from));
    dist[_from] = std::make_pair(0.0, _from);

    while (!pq.empty())
    {
      // This is the minimum distance vertex.
      VertexId u = pq.top().second;

      // Shortcut: Destination vertex found, exiting.
      if (_to != kNullId && _to == u)
        break;

      pq.pop();

      for (auto const &edgePair : _graph.IncidentsFrom(u))
      {
        const auto &edge = edgePair.second.get();
        const auto &v = edge.From(u);
        double weight = edge.Weight();

        //  If there is shorted path to v through u.
        if (dist[v].first > dist[u].first + weight)
        {
          // Updating distance of v.
          dist[v] = std::make_pair(dist[u].first + weight, u);
          pq.push(std::make_pair(dist[v].first, v));
        }
      }
    }

    return dist;
  }

}
}
}
#endif
