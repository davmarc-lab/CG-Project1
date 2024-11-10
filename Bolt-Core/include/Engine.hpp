#pragma once

#include "Application/Application.hpp"

#include "Core/Core.hpp"
#include "Core/Enums.hpp"
#include "Core/Event.hpp"
#include "Core/InputManager.hpp"
#include "Core/Layer.hpp"
#include "Core/LayerManager.hpp"
#include "Core/Log.hpp"
#include "Core/Math.hpp"
#include "Core/Physics.hpp"
#include "Core/RenderApi.hpp"
#include "Core/Resource.hpp"
#include "Core/Scene.hpp"
#include "Core/Structs.hpp"
#include "Core/Timestep.hpp"
#include "Core/Utils.hpp"

#include "ECS/Component.hpp"
#include "ECS/Entity.hpp"
#include "ECS/EntityManager.hpp"
#include "ECS/System.hpp"

#include "Graphic/Buffer/ElementBuffer.hpp"
#include "Graphic/Buffer/FrameBuffer.hpp"
#include "Graphic/Buffer/RenderBuffer.hpp"
#include "Graphic/Buffer/UniformBuffer.hpp"
#include "Graphic/Buffer/VertexArray.hpp"
#include "Graphic/Buffer/VertexBuffer.hpp"

#include "Graphic/Camera/Camera.hpp"
#include "Graphic/Renderer/Renderer.hpp"
#include "Graphic/Shader/Shader.hpp"
#include "Graphic/Text/TextManager.hpp"
#include "Graphic/Texture/Texture.hpp"

#ifdef BT_ENABLE_DEBUG
	#include "Graphic/ImGui/ImGuiDebug.hpp"
#endif

#include "Graphic/ImGuiLayer.hpp"
#include "Graphic/Window.hpp"

#include "Platform/ImGuiFactory.hpp"
#include "Platform/MeshFactory.hpp"
#include "Platform/MeshVertices.hpp"
