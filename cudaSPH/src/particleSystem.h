/*
 * Copyright 1993-2013 NVIDIA Corporation.  All rights reserved.
 *
 * Please refer to the NVIDIA end user license agreement (EULA) associated
 * with this source code for terms and conditions that govern your use of
 * this software. Any use, reproduction, disclosure, or distribution of
 * this software and related documentation outside the terms of the EULA
 * is strictly prohibited.
 *
 */

#ifndef __PARTICLESYSTEM_H__
#define __PARTICLESYSTEM_H__

#define DEBUG_GRID 0
#define DO_TIMING 0

#include <helper_functions.h>
#include "particles_kernel.cuh"
#include "vector_functions.h"

// Particle system class
class ParticleSystem
{
    public:
        ParticleSystem(uint numParticles, uint3 gridSize);
        ~ParticleSystem();

        enum ParticleConfig
        {
            CONFIG_RANDOM,
            CONFIG_GRID,
            _NUM_CONFIGS
        };

        enum ParticleArray
        {
            POSITION,
            VELOCITY,
        };

        void update(double deltaTime);
        void reset(ParticleConfig config);

        double *getArray(ParticleArray array);
        void   setArray(ParticleArray array, const double *data, int start, int count);

        int    getNumParticles() const
        {
            return m_numParticles;
        }

        void dumpGrid();
        void dumpParticles(uint start, uint count, const char *fileName);

        void setIterations(int i)
        {
            m_solverIterations = i;
        }

        void setDamping(double x)
        {
            m_params.globalDamping = x;
        }
        void setGravity(double x)
        {
            m_params.gravity = make_double3(0.0, x, 0.0);
        }

        void setCollideSpring(double x)
        {
            m_params.spring = x;
        }
        void setCollideDamping(double x)
        {
            m_params.damping = x;
        }
        void setCollideShear(double x)
        {
            m_params.shear = x;
        }
        void setCollideAttraction(double x)
        {
            m_params.attraction = x;
        }

        void setColliderPos(double3 x)
        {
            m_params.colliderPos = x;
        }

        double getParticleRadius()
        {
            return m_params.particleRadius;
        }
        double3 getColliderPos()
        {
            return m_params.colliderPos;
        }
        double getColliderRadius()
        {
            return m_params.colliderRadius;
        }
        uint3 getGridSize()
        {
            return m_params.gridSize;
        }
        double3 getWorldOrigin()
        {
            return m_params.worldOrigin;
        }
        double3 getCellSize()
        {
            return m_params.cellSize;
        }

    protected: // methods
        ParticleSystem() {}

        void _initialize(int numParticles);
        void _finalize();

        void initGrid(uint *size, double spacing, double jitter, uint numParticles);

    protected: // data
        bool m_bInitialized;
        uint m_numParticles;

        // CPU data
        double *m_hPos;              // particle positions
        double *m_hVel;              // particle velocities

        uint  *m_hParticleHash;
        uint  *m_hCellStart;
        uint  *m_hCellEnd;

        // GPU data
        double *m_dPos;
        double *m_dVel;

        double *m_dSortedPos;
        double *m_dSortedVel;

        // grid data for sorting method
        uint  *m_dGridParticleHash; // grid hash value for each particle
        uint  *m_dGridParticleIndex;// particle index for each particle
        uint  *m_dCellStart;        // index of start of each cell in sorted list
        uint  *m_dCellEnd;          // index of end of cell

        uint   m_gridSortBits;

        // params
        SimParams m_params;
        uint3 m_gridSize;
        uint m_numGridCells;

        StopWatchInterface *m_timer;

        uint m_solverIterations;
};

#endif // __PARTICLESYSTEM_H__
