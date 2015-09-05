/*
 * Copyright (C) 2015 Open Source Robotics Foundation
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
#ifndef _IGNITION_FRAME_GRAPH_HH_
#define _IGNITION_FRAME_GRAPH_HH_

#include <mutex>
#include <map>

#include <ignition/math/Pose3.hh>

namespace ignition
{
  namespace math
  {
    // Forward declaration of private data
    class FrameGraphPrivate;
    class RelativePosePrivate;

 //   class FramePrivate;

    class FramePrivate
    {
      public: FramePrivate(const std::string &_name,
                        const Pose3d& _pose,
                        const FramePrivate*_parentFrame);
      public: std::string name;
      public: Pose3d pose;
      // this is a direct pointer to the parent
      // frame, that speeds up lookup.
      public: const FramePrivate *parentFrame;

      public: std::map<std::string, const FramePrivate*> children;
    };

    class IGNITION_VISIBLE RelativePose
    {
      friend class FrameGraph;

      public: RelativePose();

      public: RelativePose(const RelativePose &_c);

      public: RelativePose& operator = (const RelativePose &_other);

      /// \brief destructor
      public : virtual ~RelativePose();

      public: bool Compute(Pose3d &_p) const;

      /// \brief private constructor.
      /// \param[in] The source frame must be a full path but
      /// the de
      private: RelativePose(std::mutex *mutex,
                            const FramePrivate *_srcFrame,
                            const FramePrivate *_dstFrame);

      private: RelativePosePrivate *dataPtr;
    };

    /// \brief A collection of Frames, and their relative poses
    class IGNITION_VISIBLE FrameGraph
    {
      /// \brief Default constructor. With the following default values:
      public: FrameGraph();

      /// \brief Destructor
      public: virtual ~FrameGraph();

      /// \brief Adds a new frame to the graph
      /// \param[in] _name The full path of the frame
      /// \param[in] _pose The pose of the frame, relative to the parent frame
      /// \param[in] _parent The parent frame's path. This path can be absolute
      /// or relative. This path must exist.
      /// \return True if the frame is valid, false otherwise.
      public: bool  AddFrame( const std::string &_name,
                              const Pose3d &_pose,
                              const std::string &_parent = "/world");

      /// \brief Computes a relative pose between 2 frames
      /// \param[in] _srcFrame The name of the source frame
      /// \param[in] _dstFrame The name of the destination frame
      /// \param[out] The pose between the frames, if it exists
      /// \return True if a pose exists beteen the frames
      public: bool Pose(const std::string &_srcFrame,
                         const std::string &_dstFrame,
                         Pose3d &_result) const;

      /// \brief This method returns a Relative pose instance that is
      /// initialized to
      public: bool RelativePoses(const std::string &_srcPath,
                                const std::string &_dstPathi,
                                RelativePose &_relativePose) const;

      /// \brief Returns a pointer to
      public: Pose3d *FramePose(const std::string &_path) const;

      private:

      /// \brief Copy Constructor (not allowed)
      /// \param[in] _copy FrameGraph to copy.
      private: FrameGraph(const FrameGraph &_copy);

      /// \brief Assignment operator (not allowed)
      /// \param[in] _assign FrameGraph to get values from
      private: FrameGraph &operator=(const FrameGraph &_assign);

      /// \internal
      /// \brief Private data pointer
      private: FrameGraphPrivate *dataPtr;
    };
  }
}

#endif
