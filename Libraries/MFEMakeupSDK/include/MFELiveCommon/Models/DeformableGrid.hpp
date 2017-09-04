//
//  Copyright (c) 2017 ModiFace Inc. All rights reserved.
//

#pragma once

#include <vector>
#include <functional>

#pragma GCC visibility push(default)

namespace MFE {
    /** \addtogroup MFELiveCommon_Models
     *  @{
     */
    
    /**
     Defines a warpable grid that can be directly used by OpenGL (or another renderer such as Vulkan, DirectX, or CPU).
     The grid consists of the vertices, texture coords, and vertex indices.
     Vertex indices are used to define the order of the vertices and allows vertices to be shared between triangles.
     
     This class is independent of OpenGL so it doesn't use any types from OpenGL.
     */
    class DeformableGrid {
    public:
        /**
         Constructor that allows you to specify the number of rows and cols, which defines the number of points
         along the rows and columns. For example, a 2x2 grid contains 4 points (vertices). The minimum accepted value
         for each dimension is 2.
         */
        DeformableGrid(int numRows, int numCols);
        
        //! Default constructor creates a 2x2 grid.
        DeformableGrid() : DeformableGrid(2, 2) {};
        
        class Point2D {
        public:
            float x;
            float y;
            
            Point2D(): x(0), y(0) {};
            Point2D(float x, float y) : x(x), y(y) {};
        };
        
        class Bounds {
        public:
            float minX;
            float minY;
            float maxX;
            float maxY;
            
            Bounds() : minX(0), minY(0), maxX(0), maxY(0) {}
        };
        
        //! Returns a reference to the vertices. Important: you can modify the existing elements but do not add or remove elements.
        std::vector<Point2D> &getVertices() {
            return mVertices;
        }
        
        //! Returns a const reference to the vertices.
        const std::vector<Point2D> &getVertices() const {
            return mVertices;
        }
        
        //! Returns a reference to the verticesTemp. Important: you can modify the existing elements but do not add or remove elements.
        std::vector<Point2D> &getVerticesTemp() {
            return mVerticesTemp;
        }
        
        //! Returns a const reference to the verticesTemp.
        const std::vector<Point2D> &getVerticesTemp() const {
            return mVerticesTemp;
        }
        
        //! Returns a const reference to the verticesOriginal.
        const std::vector<Point2D> &getVerticesOriginal() const {
            return mVerticesOriginal;
        }
        
        unsigned int getGridCols() const {
            return mGridCols;
        }
        
        unsigned int getGridRows() const {
            return mGridRows;
        }
        
        //! Returns a reference to the verticesTemp. Important: you can modify the existing elements but do not add or remove elements.
        std::vector<Point2D> &getTextureCoords() {
            return mTextureCoords;
        }
        
        //! Returns a const reference to the textureCoords.
        const std::vector<Point2D> &getTextureCoords() const {
            return mTextureCoords;
        }
        
        //! Returns a const reference to the vertexIndices.
        const std::vector<unsigned int> &getVertexIndices() const {
            return mVertexIndices;
        }
        
        //! Returns the current bounds estimate
        Bounds getCurrentBounds() const { return mCurrentBounds; }
        
        //! Updates the current bounds estimate
        void updateCurrentBoundsEstimate();
        
        /**
         Resets vertices and verticesTemp to verticesOriginal
         */
        void reset();
        
        /**
         Iterates through all current points
         */
        void iterateThroughVertices(std::function<void(const Point2D &origPt, Point2D &pt, unsigned int index)>);
        
        /**
         Iterates through all temp points
         */
        void iterateThroughVerticesTemp(std::function<void(const Point2D &origPt, Point2D &pt, unsigned int index)>);
        
        /**
         Iterates through all current points that are within region (edge inclusive)
         */
        void iterateThroughVertices(float x, float y, float w, float h, std::function<void(Point2D &pt, unsigned int num)>);
        
        /**
         Iterates through all original points that are within region (edge inclusive)
         */
        void iterateThroughVerticesTemp(float x, float y, float w, float h, std::function<void(Point2D &pt, unsigned int num)>);
        
    protected:
        void generateVerticesAndTextureCoords(int numRows, int numCols);
        void generateIndices();
        void iterateThroughRegion(float x, float y, float w, float h, std::vector<Point2D> &pts, unsigned int num,
                                  std::function<void(Point2D &pt, unsigned int num)>);
    private:
        unsigned int mNumVertices;
        unsigned int mNumIndices;
        
        /**
         The current grid vertices
         These points can be modified to produce warping effects
         */
        std::vector<Point2D> mVertices;
        
        /**
         Temporary space for working with points
         */
        std::vector<Point2D> mVerticesTemp;
        
        /**
         The original unwarped grid vertices, in the vertices coordinate system
         Do not modify
         */
        std::vector<Point2D> mVerticesOriginal;
        
        /**
         The number of columns in the grid of points
         */
        unsigned int mGridCols;
        
        /**
         The number of rows in the grid of points
         */
        unsigned int mGridRows;
        
        /**
         Texture coords, for use by grid warp filter
         Do not modify
         */
        std::vector<Point2D> mTextureCoords;
        
        /**
         Texture coords, for use by grid warp filter
         Do not modify
         */
        std::vector<unsigned int> mVertexIndices;
        
        /**
         The current estimate of the bounds of the grid.
         */
        Bounds mCurrentBounds;
    };
    
    /** @}*/
}

#pragma GCC visibility pop
