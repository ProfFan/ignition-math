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

#include <gtest/gtest.h>
#include <iostream>
#include <string>

#include "ignition/math/graph/Graph.hh"
#include "ignition/math/graph/GraphAlgorithms.hh"

using namespace ignition;
using namespace math;
using namespace graph;

/////////////////////////////////////////////////
TEST(GraphTest, Edges)
{
  // Create a graph with edges [(v0--v0), (v0-->v1), (v1-->v2), (v2-->v0)]
  DirectedGraph<int, double> graph(
  {
    {{"0", 0, 0}, {"1", 1, 1}, {"2", 2, 2}},
    {{{0, 0}, 1.0}, {{0, 1}, 2.0}, {{1, 2}, 3.0}, {{2, 0}, 4.0}}
  });

  auto edges = graph.Edges();
  EXPECT_EQ(edges.size(), 4u);

  // Check the Ids.
  for (auto i = 0; i < 4; ++i)
    EXPECT_NE(edges.find(i), edges.end());

  // Check the references.
  for (auto const &edgePair : edges)
  {
    auto &edge = edgePair.second.get();
    switch (edge.Id())
    {
      case 0:
      {
        EXPECT_EQ(edge.Tail(), 0);
        EXPECT_EQ(edge.Head(), 0);
        EXPECT_EQ(edge.Data(), 1.0);
        break;
      }
      case 1:
      {
        EXPECT_EQ(edge.Tail(), 0);
        EXPECT_EQ(edge.Head(), 1);
        EXPECT_EQ(edge.Data(), 2.0);
        break;
      }
      case 2:
      {
        EXPECT_EQ(edge.Tail(), 1);
        EXPECT_EQ(edge.Head(), 2);
        EXPECT_EQ(edge.Data(), 3.0);
        break;
      }
      case 3:
      {
        EXPECT_EQ(edge.Tail(), 2);
        EXPECT_EQ(edge.Head(), 0);
        EXPECT_EQ(edge.Data(), 4.0);
        break;
      }
      default:
        FAIL();
    };
  }
}

/////////////////////////////////////////////////
TEST(GraphTest, AdjacentsFrom)
{
  // Create a graph with edges [(v0--v0), (v0-->v1), (v1-->v2), (v2-->v0)]
  DirectedGraph<int, double> graph(
  {
    {{"0", 0, 0}, {"1", 1, 1}, {"2", 2, 2}},
    {{{0, 0}, 1.0}, {{0, 1}, 2.0}, {{1, 2}, 3.0}, {{2, 0}, 4.0}}
  });

  // Try to get the adjacents from an inexistent vertex.
  auto adjacents = graph.AdjacentsFrom(kNullId);
  EXPECT_TRUE(adjacents.empty());

  adjacents = graph.AdjacentsFrom(0);
  EXPECT_EQ(adjacents.size(), 2u);
  EXPECT_NE(adjacents.find(0), adjacents.end());
  EXPECT_NE(adjacents.find(1), adjacents.end());

  auto vertex = graph.VertexFromId(0);
  adjacents = graph.AdjacentsFrom(vertex);
  EXPECT_EQ(adjacents.size(), 2u);
  EXPECT_NE(adjacents.find(0), adjacents.end());
  EXPECT_NE(adjacents.find(1), adjacents.end());

  // Check the references.
  for (auto const &vertexPair : adjacents)
  {
    auto &neighborVertex = vertexPair.second.get();
    switch (neighborVertex.Id())
    {
      case 0:
      {
        EXPECT_EQ(neighborVertex.Name(), "0");
        EXPECT_EQ(neighborVertex.Data(), 0);
        break;
      }
      case 1:
      {
        EXPECT_EQ(neighborVertex.Name(), "1");
        EXPECT_EQ(neighborVertex.Data(), 1);
        break;
      }
      default:
        FAIL();
    };
  }
}

/////////////////////////////////////////////////
TEST(GraphTest, AdjacentsTo)
{
  // Create a graph with edges [(v0-->v0), (v0-->v1), (v1-->v2), (v2-->v1)]
  DirectedGraph<int, double> graph(
  {
    {{"0", 0, 0}, {"1", 1, 1}, {"2", 2, 2}},
    {{{0, 0}, 1.0}, {{0, 1}, 2.0}, {{1, 2}, 3.0}, {{2, 1}, 4.0}}
  });

  // Try to get the adjacents to an inexistent vertex.
  auto adjacents = graph.AdjacentsTo(kNullId);
  EXPECT_TRUE(adjacents.empty());

  adjacents = graph.AdjacentsTo(0);
  EXPECT_EQ(adjacents.size(), 1u);
  EXPECT_NE(adjacents.find(0), adjacents.end());

  adjacents = graph.AdjacentsTo(1);
  EXPECT_EQ(adjacents.size(), 2u);
  EXPECT_NE(adjacents.find(0), adjacents.end());
  EXPECT_NE(adjacents.find(2), adjacents.end());

  auto vertex = graph.VertexFromId(2);
  adjacents = graph.AdjacentsTo(vertex);
  EXPECT_EQ(adjacents.size(), 1u);
  EXPECT_NE(adjacents.find(1), adjacents.end());

  // Check the references.
  for (auto const &vertexPair : adjacents)
  {
    auto &neighborVertex = vertexPair.second.get();
    switch (neighborVertex.Id())
    {
      case 1:
      {
        EXPECT_EQ(neighborVertex.Name(), "1");
        EXPECT_EQ(neighborVertex.Data(), 1);
        break;
      }
      default:
        FAIL();
    };
  }
}

/////////////////////////////////////////////////
TEST(GraphTest, IncidentsFrom)
{
  // Create a graph with edges [(v0--v0), (v0-->v1), (v1-->v0), (v1-->v2)]
  DirectedGraph<int, double> graph(
  {
    {{"0", 0, 0}, {"1", 1, 1}, {"2", 2, 2}},
    {{{0, 0}, 1.0}, {{0, 1}, 2.0}, {{1, 0}, 3.0}, {{1, 2}, 4.0}}
  });

  auto incidents = graph.IncidentsFrom(0);
  EXPECT_EQ(incidents.size(), 2u);
  EXPECT_NE(incidents.find(0), incidents.end());
  EXPECT_NE(incidents.find(1), incidents.end());

  auto vertex = graph.VertexFromId(1);
  incidents = graph.IncidentsFrom(vertex);
  EXPECT_EQ(incidents.size(), 2u);
  EXPECT_NE(incidents.find(2), incidents.end());
  EXPECT_NE(incidents.find(3), incidents.end());

  // Check the references.
  for (auto const &edgePair : incidents)
  {
    auto &edge = edgePair.second.get();
    switch (edge.Id())
    {
      case 2:
      {
        auto vertices = edge.Vertices();
        EXPECT_EQ(vertices.first, 1);
        EXPECT_EQ(vertices.second, 0);
        EXPECT_EQ(edge.Data(), 3.0);
        break;
      }
      case 3:
      {
        auto vertices = edge.Vertices();
        EXPECT_EQ(vertices.first, 1);
        EXPECT_EQ(vertices.second, 2);
        EXPECT_EQ(edge.Data(), 4.0);
        break;
      }
      default:
        FAIL();
    };
  }

  incidents = graph.IncidentsFrom(2);
  EXPECT_TRUE(incidents.empty());
}

/////////////////////////////////////////////////
TEST(GraphTest, IncidentsTo)
{
  // Create a graph with edges [(v0--v0), (v0-->v1), (v1-->v2), (v2-->v0)]
  DirectedGraph<int, double> graph(
  {
    {{"0", 0, 0}, {"1", 1, 1}, {"2", 2, 2}},
    {{{0, 0}, 1.0}, {{0, 1}, 2.0}, {{1, 2}, 3.0}, {{2, 0}, 4.0}}
  });

  auto incidents = graph.IncidentsTo(0);
  EXPECT_EQ(incidents.size(), 2u);
  EXPECT_NE(incidents.find(0), incidents.end());
  EXPECT_NE(incidents.find(3), incidents.end());

  auto vertex = graph.VertexFromId(0);
  incidents = graph.IncidentsTo(vertex);
  EXPECT_EQ(incidents.size(), 2u);
  EXPECT_NE(incidents.find(0), incidents.end());
  EXPECT_NE(incidents.find(3), incidents.end());

  // Check the references.
  for (auto const &edgePair : incidents)
  {
    auto &edge = edgePair.second.get();
    switch (edge.Id())
    {
      case 0:
      {
        auto vertices = edge.Vertices();
        EXPECT_EQ(vertices.first, 0);
        EXPECT_EQ(vertices.second, 0);
        EXPECT_EQ(edge.Data(), 1.0);
        break;
      }
      case 3:
      {
        auto vertices = edge.Vertices();
        EXPECT_EQ(vertices.first, 2);
        EXPECT_EQ(vertices.second, 0);
        EXPECT_EQ(edge.Data(), 4.0);
        break;
      }
      default:
        FAIL();
    };
  }
}

/////////////////////////////////////////////////
TEST(GraphTest, InDegree)
{
  // Create a graph with edges [(v0--v0), (v0-->v1), (v1-->v2), (v2-->v1)]
  DirectedGraph<int, double> graph(
  {
    {{"0", 0, 0}, {"1", 1, 1}, {"2", 2, 2}},
    {{{0, 0}, 1.0}, {{0, 1}, 2.0}, {{1, 2}, 3.0}, {{2, 1}, 4.0}}
  });

  EXPECT_EQ(graph.InDegree(0), 1u);
  EXPECT_EQ(graph.InDegree(graph.VertexFromId(0)), 1u);
  EXPECT_EQ(graph.InDegree(1), 2u);
  EXPECT_EQ(graph.InDegree(graph.VertexFromId(1)), 2u);
}

/////////////////////////////////////////////////
TEST(GraphTest, OutDegree)
{
  // Create a graph with edges [(v0--v0), (v0-->v1), (v1-->v0), (v1-->v2)]
  DirectedGraph<int, double> graph(
  {
    {{"0", 0, 0}, {"1", 1, 1}, {"2", 2, 2}},
    {{{0, 0}, 1.0}, {{0, 1}, 2.0}, {{1, 0}, 3.0}, {{1, 2}, 4.0}}
  });

  EXPECT_EQ(graph.OutDegree(0), 2u);
  EXPECT_EQ(graph.OutDegree(graph.VertexFromId(0)), 2u);
  EXPECT_EQ(graph.OutDegree(1), 2u);
  EXPECT_EQ(graph.OutDegree(graph.VertexFromId(1)), 2u);
  EXPECT_EQ(graph.OutDegree(2), 0u);
  EXPECT_EQ(graph.OutDegree(graph.VertexFromId(2)), 0u);
}

/////////////////////////////////////////////////
TEST(GraphTest, AddEdge)
{
  // Create a graph with three vertices.
  DirectedGraph<int, double> graph(
  {
    {{"0", 0, 0}, {"1", 1, 1}, {"2", 2, 2}},
    {}
  });

  // Create some edges [(v0-->v1), (v1-->v2), (v2-->v0)]
  auto e0 = graph.AddEdge({0, 1}, 2.0);
  auto e1 = graph.AddEdge({1, 2}, 3.0);
  auto e2 = graph.AddEdge({2, 0}, 4.0);

  // Check the edge content.
  EXPECT_EQ(e0.Data(), 2.0);
  EXPECT_EQ(e1.Data(), 3.0);
  EXPECT_EQ(e2.Data(), 4.0);

  // Check that the edges point to the right vertices.
  EXPECT_EQ(e0.Tail(), 0);
  EXPECT_EQ(e0.Head(), 1);

  auto edges = graph.Edges();
  EXPECT_EQ(edges.size(), 3u);

  // Try to add an edge with an incorrect tail.
  auto edge = graph.AddEdge({kNullId, 1}, 2.0);
  EXPECT_EQ(edge.Id(), kNullId);
  EXPECT_EQ(graph.Edges().size(), 3u);

  // Try to add an edge with an incorrect head.
  edge = graph.AddEdge({0, kNullId}, 2.0);
  EXPECT_EQ(edge.Id(), kNullId);
  EXPECT_EQ(graph.Edges().size(), 3u);
}

/////////////////////////////////////////////////
TEST(GraphTest, RemoveEdge)
{
  // Create a graph with edges [(v0-->v1), (v1-->v2), (v2-->v0)]
  DirectedGraph<int, double> graph(
  {
    {{"0", 0, 0}, {"1", 1, 1}, {"2", 2, 2}},
    {{{0, 1}, 2.0}, {{1, 2}, 3.0}, {{2, 0}, 4.0}}
  });

  // Remove a nonexistent edge shouldn't cause any effect.
  EXPECT_FALSE(graph.RemoveEdge(kNullId));
  EXPECT_EQ(graph.Edges().size(), 3u);
  EXPECT_EQ(graph.IncidentsTo(1).size(), 1u);

  // Remove the edge (v0-->v1)
  EXPECT_TRUE(graph.RemoveEdge(0));
  EXPECT_EQ(graph.Edges().size(), 2u);
  EXPECT_EQ(graph.IncidentsTo(1).size(), 0u);

  // Remove the edge (v1-->v2)
  auto edge = graph.EdgeFromId(1);
  EXPECT_TRUE(graph.RemoveEdge(edge));
  EXPECT_EQ(graph.Edges().size(), 1u);

  // Try to remove an edge that doesn't exist anymore.
  EXPECT_FALSE(graph.RemoveEdge(1));
  EXPECT_EQ(graph.Edges().size(), 1u);

  // Remove the edge (v2-->v0)
  EXPECT_TRUE(graph.RemoveEdge(2));
  EXPECT_EQ(graph.Edges().size(), 0u);
}

/////////////////////////////////////////////////
TEST(GraphTest, RemoveVertex)
{
  // Create a graph with edges [(v0-->v1), (v1-->v2), (v2-->v0)]
  DirectedGraph<int, double> graph(
  {
    {{"0", 0, 0}, {"1", 1, 1}, {"2", 2, 2}},
    {{{0, 1}, 2.0}, {{1, 2}, 3.0}, {{2, 0}, 4.0}}
  });

  // Remove a nonexistent vertex shouldn't cause any effect.
  EXPECT_FALSE(graph.RemoveVertex(kNullId));
  EXPECT_EQ(graph.Vertices().size(), 3u);
  EXPECT_EQ(graph.AdjacentsFrom(1).size(), 1u);

  // Remove vertex #2.
  EXPECT_TRUE(graph.RemoveVertex(2));
  EXPECT_EQ(graph.Vertices().size(), 2u);
  EXPECT_EQ(graph.Edges().size(), 1u);
  EXPECT_EQ(graph.AdjacentsFrom(1).size(), 0u);

  // Remove vertex #1.
  auto vertex = graph.VertexFromId(1);
  EXPECT_TRUE(graph.RemoveVertex(vertex));
  EXPECT_EQ(graph.Vertices().size(), 1u);
  EXPECT_TRUE(graph.Edges().empty());

  // Try to remove a vertex (#1) that doesn't exist anymore.
  EXPECT_FALSE(graph.RemoveVertex(1));
  EXPECT_EQ(graph.Vertices().size(), 1u);
  EXPECT_TRUE(graph.Edges().empty());

  // Remove vertex #0.
  EXPECT_TRUE(graph.RemoveVertex(0));
  EXPECT_TRUE(graph.Vertices().empty());

  EXPECT_TRUE(graph.Empty());
}

/////////////////////////////////////////////////
TEST(GraphTest, RemoveVertices)
{
  // Create a graph with edges [(v0-->v1), (v1-->v2), (v2-->v3), (v3-->v0)]
  DirectedGraph<int, double> graph(
  {
    {{"v0", 0, 0}, {"v1", 1, 1}, {"common", 2, 2}, {"common", 3, 3}},
    {{{0, 1}, 2.0}, {{1, 2}, 3.0}, {{2, 3}, 4.0}, {{3, 0}, 5.0}}
  });

  // Try to remove a node with a name that doesn't exist.
  EXPECT_EQ(graph.RemoveVertices("wrong_name"), 0);
  EXPECT_EQ(graph.Vertices().size(), 4u);
  EXPECT_EQ(graph.AdjacentsFrom(1).size(), 1u);

  // Remove two vertices at the same time.
  EXPECT_EQ(graph.RemoveVertices("common"), 2u);
  EXPECT_EQ(graph.Vertices().size(), 2u);
  EXPECT_EQ(graph.Edges().size(), 1u);

  EXPECT_EQ(graph.AdjacentsFrom(1).size(), 0u);

  EXPECT_EQ(graph.RemoveVertices("v1"), 1u);
  EXPECT_EQ(graph.Vertices().size(), 1u);
  EXPECT_TRUE(graph.Edges().empty());

  EXPECT_EQ(graph.RemoveVertices("v0"), 1u);
  EXPECT_TRUE(graph.Vertices().empty());

  EXPECT_TRUE(graph.Empty());
}

/////////////////////////////////////////////////
TEST(GraphTest, StreamInsertion)
{
  // Create a graph with 4 vertices and
  // edges [(v0-->v0), (v0-->v1), (v1-->v2), (v2-->v3)]
  DirectedGraph<int, double> graph(
  {
    {{"v0", 0, 0}, {"v1", 1, 1}, {"v2", 2, 2}, {"v3", 3, 3}},
    {{{0, 1}, 2.0, 4.0}, {{0, 0}, 2.0, 6.0}, {{1, 2}, 3.0}, {{2, 0}, 4.0}}
  });

  std::ostringstream output;
  output << graph;

  std::cout << "# Use this snippet with your favorite DOT tool." << std::endl;
  std::cout << graph << std::endl;

  for (auto const &s : {"digraph {\n",
                        "  0 [label=\"v0 (0)\"];\n",
                        "  1 [label=\"v1 (1)\"];\n",
                        "  2 [label=\"v2 (2)\"];\n",
                        "  3 [label=\"v3 (3)\"];\n",
                        "  0 -> 1 [label=4];\n",
                        "  0 -> 0 [label=6];\n",
                        "  1 -> 2 [label=1];\n",
                        "  2 -> 0 [label=1];\n"})
  {
    EXPECT_NE(output.str().find(s), std::string::npos);
  }
}

/////////////////////////////////////////////////
TEST(GraphTest, DFS)
{
  DirectedGraph<int, double> graph(
  {
    // Vertices.
    {{"A", 0, 0}, {"B", 1, 1}, {"C", 2, 2}, {"D", 3, 3}, {"E", 4, 4},
     {"F", 5, 5}, {"G", 6, 6}},
    // Edges.
    {{{0, 1}, 2.0}, {{0, 2}, 3.0}, {{0, 4}, 4.0}, {{1, 3}, 2.0},
     {{1, 5}, 3.0}, {{2, 6}, 4.0}, {{5, 4}, 2.0}}
  });

  auto res = DFS(graph, 0);
  std::vector<VertexId> expected = {0, 4, 2, 6, 1, 5, 3};
  EXPECT_EQ(res, expected);
}
