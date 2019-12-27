//
// Copyright (c) 2008-2019 the Urho3D project.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//

/// \file

#pragma once

#include "../Glow/LightmapCharter.h"
#include "../Graphics/RenderPath.h"
#include "../Scene/Scene.h"

namespace Urho3D
{

class Camera;
class Context;

/// Lightmap seam description.
struct LightmapSeam
{
    /// Edge on lightmap UV.
    Vector2 positions_[2];
    /// Other side of the edge on lightmap UV.
    Vector2 otherPositions_[2];
    /// Transform by scale and offset.
    LightmapSeam Transformed(const Vector2& scale, const Vector2& offset) const
    {
        LightmapSeam result;
        result.positions_[0] = positions_[0] * scale + offset;
        result.positions_[1] = positions_[1] * scale + offset;
        result.otherPositions_[0] = otherPositions_[0] * scale + offset;
        result.otherPositions_[1] = otherPositions_[1] * scale + offset;
        return result;
    }
};

/// Vector of lightmap seams.
using LightmapSeamVector = ea::vector<LightmapSeam>;

/// Baking scene for single lightmap chart.
struct LightmapGeometryBakingScene
{
    /// Context.
    Context* context_{};
    /// Lightmap chart index.
    unsigned index_{};
    /// Width of the chart.
    unsigned width_{};
    /// Height of the chart.
    unsigned height_{};
    /// Size of the chart.
    IntVector2 size_;
    /// Baking scene.
    SharedPtr<Scene> scene_;
    /// Baking camera.
    Camera* camera_{};
    /// Baking render path.
    SharedPtr<RenderPath> renderPath_;
    /// Lightmap seams.
    LightmapSeamVector seams_;
};

/// Load render path.
URHO3D_API SharedPtr<RenderPath> LoadRenderPath(Context* context, const ea::string& renderPathName);

/// Generate lightmap geometry baking scene for lightmap chart.
URHO3D_API LightmapGeometryBakingScene GenerateLightmapGeometryBakingScene(Context* context,
    const LightmapChart& chart, const LightmapGeometryBakingSettings& settings, SharedPtr<RenderPath> bakeRenderPath);

/// Generate baking scenes for lightmap charts.
URHO3D_API ea::vector<LightmapGeometryBakingScene> GenerateLightmapGeometryBakingScenes(Context* context,
    const ea::vector<LightmapChart>& charts, const LightmapGeometryBakingSettings& settings);

/// Lightmap geometry buffer of lightmap chart.
struct LightmapChartGeometryBuffer
{
    /// Lightmap chart index.
    unsigned index_{};
    /// Width of the chart.
    unsigned width_{};
    /// Height of the chart.
    unsigned height_{};

    /// Positions as is.
    ea::vector<Vector3> geometryPositions_;
    /// Smooth positions after Phong tesselation.
    // TODO: Implement
    ea::vector<Vector3> smoothPositions_;
    /// Smooth normals used in rendering.
    ea::vector<Vector3> smoothNormals_;
    /// Raw face normals.
    // TODO: Implement
    ea::vector<Vector3> faceNormals_;
    /// Geometry IDs.
    ea::vector<unsigned> geometryIds_;
    /// Lightmap seams.
    LightmapSeamVector seams_;

    /// Construct default.
    LightmapChartGeometryBuffer() = default;
    /// Construct valid.
    LightmapChartGeometryBuffer(unsigned index, unsigned width, unsigned height)
        : index_(index)
        , width_(width)
        , height_(height)
        , geometryPositions_(width * height)
        , smoothPositions_(width * height)
        , smoothNormals_(width * height)
        , faceNormals_(width * height)
        , geometryIds_(width * height)
    {
    }
    /// Convert index to location.
    IntVector2 IndexToLocation(unsigned index) const
    {
        return { static_cast<int>(index % width_), static_cast<int>(index / width_) };
    }
    /// Returns whether the location is valid.
    bool IsValidLocation(const IntVector2& location) const
    {
        return 0 <= location.x_ && location.x_ < static_cast<int>(width_)
            && 0 <= location.y_ && location.y_ < static_cast<int>(height_);
    }
    /// Convert location to index.
    unsigned LocationToIndex(const IntVector2& location) const
    {
        return location.x_ + width_ * location.y_;
    }
};

/// Vector of lightmap geometry buffers.
using LightmapChartGeometryBufferVector = ea::vector<LightmapChartGeometryBuffer>;

/// Bake lightmap geometry buffer for lightmap chart.
URHO3D_API LightmapChartGeometryBuffer BakeLightmapGeometryBuffer(const LightmapGeometryBakingScene& bakingScene);

/// Bake lightmap geometry buffer for lightmap charts.
URHO3D_API LightmapChartGeometryBufferVector BakeLightmapGeometryBuffers(const ea::vector<LightmapGeometryBakingScene>& bakingScenes);

}
