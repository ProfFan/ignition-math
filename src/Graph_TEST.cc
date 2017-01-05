/*
 * Copyright (C) 2016 Open Source Robotics Foundation
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
#include <algorithm>
#include <iostream>

#include "ignition/math/Graph.hh"

using namespace ignition;
using namespace math;

/////////////////////////////////////////////////
TEST(GraphTest, VertexById)
{
  Graph<int, double> graph;

  // Create some vertexes.
  auto v0 = graph.AddVertex(0);
  ASSERT_TRUE(v0 != nullptr);
  auto v1 = graph.AddVertex(1);
  ASSERT_TRUE(v1 != nullptr);
  auto v2 = graph.AddVertex(2);
  ASSERT_TRUE(v2 != nullptr);

  auto v = graph.VertexById(v0->Id());
  ASSERT_TRUE(v != nullptr);
  EXPECT_EQ(v, v0);

  // Id not found.
  v = graph.VertexById(-1);
  ASSERT_EQ(v, nullptr);
}

/////////////////////////////////////////////////
TEST(GraphTest, Vertexes)
{
  Graph<int, double> graph;

  // Create some vertexes.
  auto v0 = graph.AddVertex(0);
  ASSERT_TRUE(v0 != nullptr);
  auto v1 = graph.AddVertex(1);
  ASSERT_TRUE(v1 != nullptr);
  auto v2 = graph.AddVertex(2);
  ASSERT_TRUE(v2 != nullptr);

  auto vertexes = graph.Vertexes();
  EXPECT_EQ(vertexes.size(), 3u);
  // Check that the pointers point to the same vertexes.
  EXPECT_NE(std::find(vertexes.begin(), vertexes.end(), v0), vertexes.end());
  EXPECT_NE(std::find(vertexes.begin(), vertexes.end(), v1), vertexes.end());
  EXPECT_NE(std::find(vertexes.begin(), vertexes.end(), v2), vertexes.end());
}

/////////////////////////////////////////////////
TEST(GraphTest, Edges)
{
  Graph<int, double> graph;

  // Create some vertexes.
  auto v0 = graph.AddVertex(0);
  ASSERT_TRUE(v0 != nullptr);
  auto v1 = graph.AddVertex(1);
  ASSERT_TRUE(v1 != nullptr);
  auto v2 = graph.AddVertex(2);
  ASSERT_TRUE(v2 != nullptr);

  // Create some edges [(v0-->v1), (v1-->v2). (v2-->v0)]
  auto e0 = graph.AddEdge(v0, v1, 2.0);
  ASSERT_TRUE(e0 != nullptr);
  auto e1 = graph.AddEdge(v1, v2, 3.0);
  ASSERT_TRUE(e1 != nullptr);
  auto e2 = graph.AddEdge(v2, v0, 4.0);
  ASSERT_TRUE(e2 != nullptr);

  auto edges = graph.Edges();
  EXPECT_EQ(edges.size(), 3u);
  // Check that the pointers point to the same edges.
  EXPECT_NE(std::find(edges.begin(), edges.end(), e0), edges.end());
  EXPECT_NE(std::find(edges.begin(), edges.end(), e1), edges.end());
  EXPECT_NE(std::find(edges.begin(), edges.end(), e2), edges.end());
}

/////////////////////////////////////////////////
TEST(GraphTest, EdgesWithIDs)
{
  Graph<int, double> graph;

  // Create some vertexes.
  auto v0 = graph.AddVertex(0);
  ASSERT_TRUE(v0 != nullptr);
  auto v1 = graph.AddVertex(1);
  ASSERT_TRUE(v1 != nullptr);
  auto v2 = graph.AddVertex(2);
  ASSERT_TRUE(v2 != nullptr);

  // Create some edges [(v0-->v1), (v1-->v2). (v2-->v0)]
  auto e0 = graph.AddEdge(0, 1, 2.0);
  ASSERT_TRUE(e0 != nullptr);
  auto e1 = graph.AddEdge(1, 2, 3.0);
  ASSERT_TRUE(e1 != nullptr);
  auto e2 = graph.AddEdge(2, 0, 4.0);
  ASSERT_TRUE(e2 != nullptr);

  // Try to add a repeated edge.
  auto invalid = graph.AddEdge(2, 0, 5.0);
  ASSERT_TRUE(invalid == nullptr);

  auto edges = graph.Edges();
  EXPECT_EQ(edges.size(), 3u);

  // Check that the pointers point to the same edges.
  EXPECT_NE(std::find(edges.begin(), edges.end(), e0), edges.end());
  EXPECT_NE(std::find(edges.begin(), edges.end(), e1), edges.end());
  EXPECT_NE(std::find(edges.begin(), edges.end(), e2), edges.end());
}

/////////////////////////////////////////////////
TEST(GraphTest, Empty)
{
  Graph<int, double> graph;

  EXPECT_TRUE(graph.Empty());

  // Create a vertex.
  auto v0 = graph.AddVertex(0);
  ASSERT_TRUE(v0 != nullptr);

  EXPECT_FALSE(graph.Empty());
}

/////////////////////////////////////////////////
TEST(GraphTest, Adjacents)
{
  Graph<int, double> graph;

  // Create some vertexes.
  auto v0 = graph.AddVertex(0);
  ASSERT_TRUE(v0 != nullptr);
  auto v1 = graph.AddVertex(1);
  ASSERT_TRUE(v1 != nullptr);
  auto v2 = graph.AddVertex(2);
  ASSERT_TRUE(v2 != nullptr);

  // Create some edges [(v0-->v1), (v1-->v2). (v2-->v0)]
  auto e0 = graph.AddEdge(v0, v1, 2.0);
  ASSERT_TRUE(e0 != nullptr);
  auto e1 = graph.AddEdge(v1, v2, 3.0);
  ASSERT_TRUE(e1 != nullptr);
  auto e2 = graph.AddEdge(v2, v0, 4.0);
  ASSERT_TRUE(e2 != nullptr);

  auto adjacents = graph.Adjacents(v0);
  EXPECT_EQ(adjacents.size(), 1u);
  EXPECT_NE(std::find(adjacents.begin(), adjacents.end(), v1), adjacents.end());

  adjacents = graph.Adjacents(0);
  EXPECT_EQ(adjacents.size(), 1u);
  EXPECT_NE(std::find(adjacents.begin(), adjacents.end(), v1), adjacents.end());
}

/////////////////////////////////////////////////
TEST(GraphTest, Incidents)
{
  Graph<int, double> graph;

  // Create some vertexes.
  auto v0 = graph.AddVertex(0);
  ASSERT_TRUE(v0 != nullptr);
  auto v1 = graph.AddVertex(1);
  ASSERT_TRUE(v1 != nullptr);
  auto v2 = graph.AddVertex(2);
  ASSERT_TRUE(v2 != nullptr);

  // Create some edges [(v0-->v1), (v1-->v2). (v2-->v0)]
  auto e0 = graph.AddEdge(v0, v1, 2.0);
  ASSERT_TRUE(e0 != nullptr);
  auto e1 = graph.AddEdge(v1, v2, 3.0);
  ASSERT_TRUE(e1 != nullptr);
  auto e2 = graph.AddEdge(v2, v0, 4.0);
  ASSERT_TRUE(e2 != nullptr);

  auto incidents = graph.Incidents(v0);
  EXPECT_EQ(incidents.size(), 1u);
  EXPECT_NE(std::find(incidents.begin(), incidents.end(), e2), incidents.end());

  incidents = graph.Incidents(0);
  EXPECT_EQ(incidents.size(), 1u);
  EXPECT_NE(std::find(incidents.begin(), incidents.end(), e2), incidents.end());
}

/////////////////////////////////////////////////
TEST(GraphTest, AddVertex)
{
  Graph<int, double> graph;

  // Create some vertexes without Id.
  auto v0 = graph.AddVertex(0);
  ASSERT_TRUE(v0 != nullptr);
  auto v1 = graph.AddVertex(1);
  ASSERT_TRUE(v1 != nullptr);
  auto v2 = graph.AddVertex(2);
  ASSERT_TRUE(v2 != nullptr);

  // Create a vertex with Id.
  auto v3 = graph.AddVertex(5, 3);
  ASSERT_TRUE(v3 != nullptr);
  EXPECT_EQ(v3->Id(), 3);
  EXPECT_EQ(v3->Data(), 5);

  // Create a vertex with an already used Id.
  auto v4 = graph.AddVertex(0, 3);
  ASSERT_TRUE(v4 == nullptr);

  auto vertexes = graph.Vertexes();
  EXPECT_EQ(vertexes.size(), 4u);
}

/////////////////////////////////////////////////
TEST(GraphTest, AddEdge)
{
  Graph<int, double> graph;

  // Create some vertexes without Id.
  auto v0 = graph.AddVertex(0);
  ASSERT_TRUE(v0 != nullptr);
  auto v1 = graph.AddVertex(1);
  ASSERT_TRUE(v1 != nullptr);
  auto v2 = graph.AddVertex(2);
  ASSERT_TRUE(v2 != nullptr);

  // Create some edges [(v0-->v1), (v1-->v2). (v2-->v0)]
  auto e0 = graph.AddEdge(v0, v1, 2.0);
  ASSERT_TRUE(e0 != nullptr);
  auto e1 = graph.AddEdge(v1, v2, 3.0);
  ASSERT_TRUE(e1 != nullptr);
  auto e2 = graph.AddEdge(v2, v0, 4.0);
  ASSERT_TRUE(e2 != nullptr);

  // Check the edge content.
  EXPECT_EQ(e0->Data(), 2.0);
  EXPECT_EQ(e1->Data(), 3.0);
  EXPECT_EQ(e2->Data(), 4.0);

  // Check that the edges point to the right vertexes.
  EXPECT_EQ(e0->Tail(), v0);
  EXPECT_EQ(e0->Head(), v1);

  auto edges = graph.Edges();
  EXPECT_EQ(edges.size(), 3u);

  // Try to add an edge with an incorrect tail.
  auto edge = graph.AddEdge(nullptr, v1, 2.0);
  EXPECT_EQ(edge, nullptr);
  EXPECT_EQ(edges.size(), 3u);

  // Try to add an edge with an incorrect head.
  edge = graph.AddEdge(v0, nullptr, 2.0);
  EXPECT_EQ(edge, nullptr);
  EXPECT_EQ(edges.size(), 3u);
}

/////////////////////////////////////////////////
TEST(GraphTest, RemoveEdge)
{
  Graph<int, double> graph;

  // Create some vertexes without Id.
  auto v0 = graph.AddVertex(0);
  ASSERT_TRUE(v0 != nullptr);
  auto v1 = graph.AddVertex(1);
  ASSERT_TRUE(v1 != nullptr);
  auto v2 = graph.AddVertex(2);
  ASSERT_TRUE(v2 != nullptr);

  // Create some edges [(v0-->v1), (v1-->v2). (v2-->v0)]
  auto e0 = graph.AddEdge(v0, v1, 2.0);
  ASSERT_TRUE(e0 != nullptr);
  auto e1 = graph.AddEdge(v1, v2, 3.0);
  ASSERT_TRUE(e1 != nullptr);
  auto e2 = graph.AddEdge(v2, v0, 4.0);
  ASSERT_TRUE(e2 != nullptr);

  EXPECT_EQ(graph.Edges().size(), 3u);

  // Remove using nullptr shouldn't cause any effect.
  EdgePtr<int, double> edge;
  graph.RemoveEdge(edge);
  EXPECT_EQ(graph.Edges().size(), 3u);

  EXPECT_EQ(graph.Incidents(v1).size(), 1);

  EXPECT_NE(e0->Head(), nullptr);
  EXPECT_NE(e0->Tail(), nullptr);

  graph.RemoveEdge(e0);
  EXPECT_EQ(graph.Edges().size(), 2u);
  // After disconnecting e0, it shoudln't be possible to reach the vertexes.
  EXPECT_EQ(e0->Head(), nullptr);
  EXPECT_EQ(e0->Tail(), nullptr);

  EXPECT_EQ(graph.Incidents(v1).size(), 0);

  graph.RemoveEdge(e1);
  EXPECT_EQ(graph.Edges().size(), 1u);

  // Try to remove an edge that doesn't exist.
  graph.RemoveEdge(e1);
  EXPECT_EQ(graph.Edges().size(), 1u);

  graph.RemoveEdge(e2);
  EXPECT_EQ(graph.Edges().size(), 0u);
}

/////////////////////////////////////////////////
TEST(GraphTest, RemoveVertex)
{
  Graph<int, double> graph;

  // Create some vertexes without Id.
  auto v0 = graph.AddVertex(0);
  ASSERT_TRUE(v0 != nullptr);
  auto v1 = graph.AddVertex(1);
  ASSERT_TRUE(v1 != nullptr);
  auto v2 = graph.AddVertex(2);
  ASSERT_TRUE(v2 != nullptr);

  // Create some edges [(v0-->v1), (v1-->v2). (v2-->v0)]
  auto e0 = graph.AddEdge(v0, v1, 2.0);
  ASSERT_TRUE(e0 != nullptr);
  auto e1 = graph.AddEdge(v1, v2, 3.0);
  ASSERT_TRUE(e1 != nullptr);
  auto e2 = graph.AddEdge(v2, v0, 4.0);
  ASSERT_TRUE(e2 != nullptr);

  EXPECT_EQ(graph.Edges().size(), 3u);

  // Remove using nullptr shouldn't cause any effect.
  VertexPtr<int> vertex;
  graph.RemoveVertex(vertex);
  EXPECT_EQ(graph.Vertexes().size(), 3u);

  EXPECT_EQ(graph.Adjacents(v1).size(), 1);

  graph.RemoveVertex(2);
  EXPECT_EQ(graph.Vertexes().size(), 2u);
  EXPECT_EQ(graph.Edges().size(), 1u);

  EXPECT_EQ(graph.Adjacents(v1).size(), 0);

  graph.RemoveVertex(v1);
  EXPECT_EQ(graph.Vertexes().size(), 1u);
  EXPECT_TRUE(graph.Edges().empty());

  graph.RemoveVertex(v0);
  EXPECT_TRUE(graph.Vertexes().empty());

  EXPECT_TRUE(graph.Empty());
}

/////////////////////////////////////////////////
TEST(GraphTest, StreamInsertion)
{
  Graph<int, double> graph;

  // Create some vertexes without Id.
  auto v0 = graph.AddVertex(0);
  ASSERT_TRUE(v0 != nullptr);
  auto v1 = graph.AddVertex(1);
  ASSERT_TRUE(v1 != nullptr);
  auto v2 = graph.AddVertex(2);
  ASSERT_TRUE(v2 != nullptr);

  // Create some edges [(v0-->v1), (v1-->v2). (v2-->v0)]
  auto e0 = graph.AddEdge(v0, v1, 2.0);
  ASSERT_TRUE(e0 != nullptr);
  auto e1 = graph.AddEdge(v1, v2, 3.0);
  ASSERT_TRUE(e1 != nullptr);
  auto e2 = graph.AddEdge(v2, v0, 4.0);
  ASSERT_TRUE(e2 != nullptr);

  EXPECT_EQ(graph.Edges().size(), 3u);

  std::ostringstream output;
  output << graph;
  std::string expectedOutput =
    "Vertexes\n"
    "  [0]\n"
    "  [1]\n"
    "  [2]\n"
    "Edges\n"
    "  [0-->1]\n"
    "  [1-->2]\n"
    "  [2-->0]\n";
  EXPECT_EQ(output.str(), expectedOutput);
}