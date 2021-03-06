cmake_minimum_required(VERSION 3.10)
project(brakeza3d)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-O3")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${brakeda3d_SOURCE_DIR}/cmake")

set(SDL2_dir /Library/Frameworks/SDL2.framework/Headers)
set(SDL2_image_dir /Library/Frameworks/SDL2_image.framework/Headers)
set(SDL2_ttf_dir /Library/Frameworks/SDL2_ttf.framework/Headers)
set(SDL2_mixer_dir /Library/Frameworks/SDL2_mixer.framework/Headers)
set(OpenGL_dir /Library/Frameworks/OpenGL.framework/Headers/)

set(CMAKE_VERBOSE_MAKEFILE ON)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(OpenGL REQUIRED)
find_package(Bullet REQUIRED)
find_package(OpenCL REQUIRED)

include_directories(
        ${SDL2_dir}
        ${SDL2_image_dir}
        ${SDL2_ttf_dir}
        ${SDL2_mixer_dir}
        ${OPENGL_INCLUDE_DIRS}
        ${BULLET_INCLUDE_DIRS}
        ${OpenCL_INCLUDE_DIRS}
)

add_executable(brakeza3d
        main.cpp
        src/Objects/Camera3D.cpp
        headers/Objects/Camera3D.h
        src/Input/InputController.cpp
        headers/Input/InputController.h
        src/Objects/Mesh3D.cpp
        headers/Objects/Mesh3D.h
        src/Objects/Triangle3D.cpp
        headers/Objects/Triangle3D.h
        src/Objects/Vertex3D.cpp
        headers/Objects/Vertex3D.h
        src/Objects/Vector3D.cpp
        headers/Objects/Vector3D.h
        src/Objects/Point2D.cpp
        headers/Objects/Point2D.h
        src/Render/Tools.cpp
        headers/Render/Tools.h
        src/Objects/Line2D.cpp
        headers/Objects/Line2D.h
        src/Misc/Color.cpp
        headers/Misc/Color.h
        src/Render/Engine.cpp
        headers/OpenCL/OCLTriangle.h
        headers/Render/Engine.h
        headers/Render/EngineSetup.h
        src/Render/EngineSetup.cpp
        src/Render/Texture.cpp
        headers/Render/Texture.h
        headers/Render/EngineBuffers.h
        src/Render/EngineBuffers.cpp
        src/Render/Frustum.cpp
        headers/Render/Frustum.h
        src/Render/Plane.cpp
        headers/Render/Plane.h
        src/Objects/Object3D.cpp
        headers/Objects/Object3D.h
        src/Render/Timer.cpp
        headers/Render/Timer.h
        headers/Render/Transforms.h
        src/Render/Transforms.cpp
        src/Render/Drawable.cpp
        headers/Render/Drawable.h
        src/Objects/BSPEntity3D.cpp
        headers/Objects/BSPEntity3D.h
        src/Objects/LightPoint3D.cpp
        headers/Objects/LightPoint3D.h

        src/Game/Game.cpp headers/Game/Game.h
        src/Render/Billboard.cpp
        headers/Render/Billboard.h
        src/Render/M3.cpp
        headers/Render/M3.h

        imgui/imconfig.h
        imgui/imgui.h
        imgui/imgui.cpp
        imgui/imgui_demo.cpp
        imgui/imgui_draw.cpp
        imgui/imgui_internal.h
        imgui/imgui_widgets.cpp
        imgui/imstb_rectpack.h
        imgui/imstb_textedit.h
        imgui/imstb_truetype.h

        imgui/examples/imgui_impl_sdl.h
        imgui/examples/imgui_impl_sdl.cpp
        imgui/examples/imgui_impl_opengl2.h
        imgui/examples/imgui_impl_opengl2.cpp

        headers/GUI/GUI.h
        headers/GUI/GUI_Menu.h
        headers/GUI/GUI_Objects3D.h
        headers/GUI/GUIManager.h
        headers/GUI/GUI_Lights.h
        headers/GUI/GUI_Log.h
        headers/GUI/GUI_Tiles.h
        headers/GUI/GUI_Weapons.h
        src/Render/Logging.cpp

        headers/Objects/Tile.h

        headers/Render/Logging.h
        src/Objects/SpriteDirectional3D.cpp
        headers/Objects/SpriteDirectional3D.h
        src/2D/TextureAnimationDirectional.cpp
        headers/2D/TextureAnimationDirectional.h
        src/Objects/Sprite3D.cpp
        headers/Objects/Sprite3D.h
        src/2D/TextureAnimation.cpp
        headers/2D/TextureAnimation.h
        src/Render/Maths.cpp
        headers/Render/Maths.h

        src/Render/BSPMap.cpp
        headers/Render/BSPMap.h
        headers/GUI/GUI_Camera.h
        src/Physics/Mesh3DBody.cpp
        headers/Physics/Mesh3DBody.h
        src/Render/PhysicsDebugDraw.cpp
        headers/Render/PhysicsDebugDraw.h
        src/Physics/Mesh3DGhost.cpp
        headers/Physics/Mesh3DGhost.h
        headers/Misc/cJSON.h
        src/Misc/cJSON.c
        src/Physics/SpriteDirectional3DBody.cpp
        headers/Physics/SpriteDirectional3DBody.h
        src/2D/WeaponsManager.cpp
        headers/2D/WeaponsManager.h
        src/2D/WeaponAnimation.cpp
        headers/2D/WeaponAnimation.h
        src/2D/WeaponType.cpp
        headers/2D/WeaponType.h
        src/2D/MenuManager.cpp
        headers/2D/MenuManager.h
        src/2D/MenuOption.cpp
        headers/2D/MenuOption.h
        src/Game/Enemy.cpp
        headers/Game/Enemy.h
        src/Brakeza3D.cpp
        headers/Brakeza3D.h
        src/Collisions/CollisionsManager.cpp
        headers/Collisions/CollisionsManager.h
        src/Game/NPCEnemyBody.cpp
        headers/Game/NPCEnemyBody.h
        src/Collisions/CollisionResolver.cpp
        headers/Collisions/CollisionResolver.h
        src/Game/Projectile3DBody.cpp
        headers/Game/Projectile3DBody.h
        headers/Collisions/CollisionResolverBetweenProjectileAndNPCEnemy.h
        headers/Collisions/CollisionResolverBetweenCamera3DAndFuncDoor.h
        headers/Collisions/CollisionResolverBetweenCamera3DAndFuncButton.h
        headers/Collisions/CollisionResolverBetweenProjectileAndBSPMap.h
        headers/Collisions/CollisionResolverBetweenEnemyPartAndBSPMap.h
        headers/Collisions/CollisionResolverBetweenProjectileAndPlayer.h
        src/Objects/Decal.cpp
        headers/Objects/Decal.h
        src/Game/NPCEnemyPartBody.cpp
        headers/Game/NPCEnemyPartBody.h
        src/Physics/Sprite3DBody.cpp
        headers/Physics/Sprite3DBody.h
        src/Physics/Body.cpp
        headers/Physics/Body.h
        headers/Objects/Cube3D.cpp
        headers/Objects/Cube3D.h
        src/Recastnavigation/RecastWrapper.cpp
        headers/Recastnavigation/RecastWrapper.h
        src/Recastnavigation/RecastGeometry.cpp
        headers/Recastnavigation/RecastGeometry.h
        src/Recastnavigation/RecastGeometryLoader.cpp
        headers/Recastnavigation/RecastGeometryLoader.h
        src/Recastnavigation/RecastBuildContext.cpp headers/Recastnavigation/RecastBuildContext.h

        headers/Game/Player.cpp src/Game/Player.h
        src/Demo.cpp headers/Demo.h
        src/Game/GameInputController.cpp headers/Game/GameInputController.h src/Misc/Counter.cpp headers/Misc/Counter.h headers/2D/TexturePackage.cpp headers/2D/TexturePackage.h src/Misc/SoundPackage.cpp headers/Misc/SoundPackage.h headers/Misc/Parallells.h)


target_link_libraries(brakeza3d
        ${SDL2_LIBRARY}
        ${SDL2_IMAGE_LIBRARY}
        ${SDLTTF_LIBRARY}
        ${SDL2_MIXER_LIBRARY}
        ${OPENGL_LIBRARIES}
        ${BULLET_LIBRARIES}
        ${OpenCL_LIBRARY}
        DebugUtils Detour DetourCrowd DetourTileCache Recast
)
