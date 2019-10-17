#pragma once
#include <windows.h>

class btDefaultCollisionConfiguration;
class btCollisionDispatcher;
struct btDbvtBroadphase;
class btSequentialImpulseConstraintSolver;
class btDiscreteDynamicsWorld;

class physicsInterface/* : public systemBase */
{
public:
    static __declspec(dllexport) void createSimulator();
    static __declspec(dllexport) void addPhysicsObject();

private:

    static btDefaultCollisionConfiguration * m_collisionConfiguration;
    static btCollisionDispatcher * m_dispatcher;
    static btDbvtBroadphase * m_broadphase;
    static btSequentialImpulseConstraintSolver * m_solver;
    static btDiscreteDynamicsWorld * m_dynamicsWorld;

};
