#include "EditorLayer.h"
#include "LevelEditorLayer.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/matrix_decompose.hpp>

namespace LevelEditor {
	EditorLayer::EditorLayer(std::shared_ptr<LevelEditorLayer>& layer)
		: Layer("Editor"), _LevelEditor(layer)
	{
		_Scene.reset(&EngineScene::Get());
		_Camera = layer->GetActiveCamera();
	}

	bool DecomposeMatrix(const glm::mat4& transform,
		glm::vec3& outTranslation,
		glm::quat& outRotation,
		glm::vec3& outScale)
	{
		glm::mat4 localMatrix = transform;

		if (glm::epsilonEqual(localMatrix[3][3], 0.0f, glm::epsilon<float>()))
			return false;

		outTranslation = glm::vec3(localMatrix[3]);
		localMatrix[3] = glm::vec4(0, 0, 0, localMatrix[3].w);

		outScale.x = glm::length(localMatrix[0]);
		localMatrix[0] = glm::normalize(localMatrix[0]);

		outScale.y = glm::length(localMatrix[1]);
		localMatrix[1] = glm::normalize(localMatrix[1]);

		outScale.z = glm::length(localMatrix[2]);
		localMatrix[2] = glm::normalize(localMatrix[2]);

		glm::mat3 rotationMatrix(localMatrix);

		outRotation = glm::quat_cast(rotationMatrix);

		return true;
	}

	void EditorLayer::DrawEntityGizmos(entt::entity selectedEntity) {
		if (selectedEntity != entt::null) {
			auto& transform = EngineScene::Get().GetRegistry().get<Engine::TransformComponent>(selectedEntity);

			glm::mat4 modelMatrix = glm::mat4(1.0f);
			modelMatrix = glm::translate(modelMatrix, transform.Position);
			modelMatrix = glm::rotate(modelMatrix, glm::radians(transform.Rotation.x), glm::vec3(1, 0, 0));
			modelMatrix = glm::rotate(modelMatrix, glm::radians(transform.Rotation.y), glm::vec3(0, 1, 0));
			modelMatrix = glm::rotate(modelMatrix, glm::radians(transform.Rotation.z), glm::vec3(0, 0, 1));
			modelMatrix = glm::scale(modelMatrix, transform.Scale);

			ImGuizmo::SetOrthographic(false);
			ImGuizmo::BeginFrame();
			ImGuizmo::SetDrawlist();

			ImGuizmo::SetRect(_ViewportPanelPos.x, _ViewportPanelPos.x, _ViewportPanelSize.x, _ViewportPanelSize.y);
			glm::mat4 viewMatrix = _Camera->GetViewMatrix();
			glm::mat4 projectionMatrix = _Camera->GetProjectionMatrix(0.1f, 100.0f);

			ImGuizmo::Manipulate(glm::value_ptr(viewMatrix), glm::value_ptr(projectionMatrix),
				_GizmoType, ImGuizmo::LOCAL, glm::value_ptr(modelMatrix));

			if (ImGuizmo::IsUsing()) {
				glm::vec3 newPosition;
				glm::vec3 newRotation;
				glm::vec3 newScale;

				glm::quat quaternion = glm::quat(glm::radians(newRotation));

				if (DecomposeMatrix(modelMatrix, newPosition, quaternion, newScale)) {
					transform.Position = newPosition;

					glm::vec3 eulerAngles = glm::eulerAngles(quaternion);
					transform.Rotation = eulerAngles;
					transform.Scale = newScale;
				}
			}
		}
	}

	void EditorLayer::OnImGuiRender()
	{
		auto& registry = _Scene->GetRegistry();
		auto screenTexture = EngineRenderer::GetScreenTexture();
		uint32_t textureID = screenTexture->GetTextureID();
		GLFWwindow* nativeWindow = (GLFWwindow*)EngineApp::Get().GetWindow().GetNativeWindow();
		int xPos, yPos;

		glfwGetWindowPos(nativeWindow, &xPos, &yPos);
		ImVec2 windowPos = ImVec2((float)xPos, (float)yPos);

		ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
		DrawTopBar();

		ImGui::SetNextWindowPos(ImVec2(windowPos.x, windowPos.y + 30.0f), ImGuiCond_Always);
		ImGui::SetNextWindowSize(ImVec2((float)EngineApp::Get().GetWindow().GetWidth(), (float)EngineApp::Get().GetWindow().GetHeight() - 30.0f), ImGuiCond_Always);
		ImGui::Begin("DockSpace", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);

		ImGuiID dockspaceId = ImGui::GetID("DockSpace");
		ImGui::DockSpace(dockspaceId, ImVec2(0, 0), ImGuiDockNodeFlags_None);

		ImGui::End();

		if (ImGui::DockBuilderGetNode(dockspaceId) == nullptr) {
			ImGui::DockBuilderAddNode(dockspaceId, ImGuiDockNodeFlags_DockSpace);
			ImGui::DockBuilderSetNodeSize(dockspaceId, ImVec2(1024, 768));

			ImGuiID viewportDock = ImGui::DockBuilderAddNode(ImGui::GetID("ViewportNode"), ImGuiDockNodeFlags_None);
			ImGuiID sceneDock = ImGui::DockBuilderAddNode(ImGui::GetID("SceneNode"), ImGuiDockNodeFlags_None);

			ImGui::DockBuilderDockWindow("Viewport", viewportDock);
			ImGui::DockBuilderDockWindow("Scene", sceneDock);

			ImGui::DockBuilderFinish(dockspaceId);
		}

		ImGui::SetNextWindowSize(ImVec2((float)EngineApp::Get().GetWindow().GetWidth() / 2, (float)EngineApp::Get().GetWindow().GetHeight()), ImGuiCond_Always);

		ImGui::Begin("Viewport", nullptr, ImGuiWindowFlags_NoResize);
		if (_LevelEditor->GetActiveCamera() != NULL) {
			_ViewportPanelSize = ImGui::GetContentRegionAvail();
			_ViewportPanelPos = ImGui::GetCursorScreenPos();

			ImGui::Image((void*)(intptr_t)textureID, _ViewportPanelSize, ImVec2(0, 1), ImVec2(1, 0));
			_LevelEditor->GetActiveCamera()->SetAspectRatio((float)_ViewportPanelSize.x / (float)_ViewportPanelSize.y);

			_LevelEditor->_ViewportFocused = ImGui::IsWindowFocused();
			_ViewportHovered = ImGui::IsWindowHovered();
			DrawEntityGizmos(_SelectedEntity);
		}
		else {
			ImGui::Text("No Active Camera!");
		}
		ImGui::End();

		DrawScene();
	}

	static bool bShowOpenSceneDialog = false;
	static bool bShowSaveSceneDialog = false;
	void EditorLayer::DrawTopBar()
	{
		ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f), ImGuiCond_Always);
		ImGui::SetNextWindowSize(ImVec2((float)EngineApp::Get().GetWindow().GetWidth(), 30.0f), ImGuiCond_Always);
		ImGui::Begin("TopBar", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar);

		if (ImGui::BeginMainMenuBar()) {
			if (ImGui::BeginMenu("File")) {
				if (ImGui::MenuItem("New")) {

				}
				if (ImGui::MenuItem("Open")) {
					bShowOpenSceneDialog = !bShowOpenSceneDialog;
				}
				if (ImGui::MenuItem("Save")) {
					if (_Scene->GetPath().empty()) {
						bShowSaveSceneDialog = !bShowSaveSceneDialog;
					}
					else {
						//EngineSaveScene(_Scene->GetPath(), *_Scene);
					}
				}
				if (ImGui::MenuItem("Save As")) {
					bShowSaveSceneDialog = !bShowSaveSceneDialog;
				}
				if (ImGui::MenuItem("Close")) {
					EngineApp::Get().Close();
				}
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Edit")) {
				if (ImGui::MenuItem("Undo")) {

				}
				if (ImGui::MenuItem("Redo")) {

				}
				ImGui::EndMenu();
			}

			ImGui::EndMainMenuBar();
		}

		IGFD::FileDialogConfig config;
		if (bShowOpenSceneDialog) {
			ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose a scene file", ".json", config);
			if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey")) {
				if (ImGuiFileDialog::Instance()->IsOk()) {
					std::string filePath = ImGuiFileDialog::Instance()->GetFilePathName();
					//EngineLoadScene(filePath, *_Scene);
				}
				ImGuiFileDialog::Instance()->Close();
				bShowOpenSceneDialog = false;
			}
		}

		if (bShowSaveSceneDialog) {
			ImGuiFileDialog::Instance()->OpenDialog("SaveFileDlgKey", "Save scene", ".json", config);
			if (ImGuiFileDialog::Instance()->Display("SaveFileDlgKey")) {
				if (ImGuiFileDialog::Instance()->IsOk()) {
					std::string filePath = ImGuiFileDialog::Instance()->GetFilePathName();
					//EngineSaveScene(filePath, *_Scene);
				}
				ImGuiFileDialog::Instance()->Close();
				bShowSaveSceneDialog = false;
			}
		}

		ImGui::SameLine();
		ImGui::Text("Level Editor");

		ImGui::End();
	}

	static bool bShowFileDialog = false;
	void EditorLayer::DrawScene()
	{
		ImGui::SetNextWindowSize(ImVec2((float)EngineApp::Get().GetWindow().GetWidth() / 2, (float)EngineApp::Get().GetWindow().GetHeight()), ImGuiCond_Always);
		ImGui::Begin("Scene", nullptr, ImGuiWindowFlags_NoResize);

		if (ImGui::Button("Create Entity")) {
			EngineEntity newEntity = _Scene->CreateEntity();
		}

		auto& registry = _Scene->GetRegistry();
		auto view = registry.view<Engine::TransformComponent, Engine::NameComponent>();
		for (auto entity : view) {
			auto& nameComp = registry.get<Engine::NameComponent>(entity);

			if (ImGui::Selectable(nameComp.name.c_str())) {
				_SelectedEntity = entity;
			}

		}
		if (_SelectedEntity != entt::null) {
			auto& nameComp = registry.get<Engine::NameComponent>(_SelectedEntity);
			auto& transform = registry.get<Engine::TransformComponent>(_SelectedEntity);

			if (ImGui::Begin("Properties")) {
				static int SelectedComboBoxComponentItem = 0;
				static int CollisionEnabled = 1;
				char buffer[128];
				strncpy(buffer, nameComp.name.c_str(), sizeof(buffer));
				if (ImGui::InputText("Name", buffer, sizeof(buffer))) {
					nameComp.name = buffer;
				}
				ImGui::SameLine(0.0f, 5.0f);
				if (ImGui::Button("Delete")) {
					registry.destroy(_SelectedEntity);
					_SelectedEntity = entt::null;
				}

				ImGui::DragFloat3("Position", glm::value_ptr(transform.Position), 0.1f);
				ImGui::DragFloat3("Rotation", glm::value_ptr(transform.Rotation), 0.1f);
				ImGui::DragFloat3("Scale", glm::value_ptr(transform.Scale), 0.1f);

				if (registry.any_of<EngineStaticMeshComponent>(_SelectedEntity)) {
					auto& staticMesh = registry.get<EngineStaticMeshComponent>(_SelectedEntity);
					ImGui::Text(staticMesh.GetModelPath().c_str());
					if (ImGui::Button("Load Model")) {
						bShowFileDialog = !bShowFileDialog;
					}
					ImGui::Checkbox("Visible", &staticMesh.bVisible);
					ImGui::Checkbox("Apply Shadows", &staticMesh.bDrawShadows);

					if (bShowFileDialog) {
						IGFD::FileDialogConfig config;

						ImGuiFileDialog::Instance()->OpenDialog("SelectModel", "Choose Texture", ".obj,.gltf", config);

						if (ImGuiFileDialog::Instance()->Display("SelectModel")) {
							if (ImGuiFileDialog::Instance()->IsOk()) {
								std::string path = ImGuiFileDialog::Instance()->GetFilePathName();
								if (!path.empty()) {
									std::replace(path.begin(), path.end(), '\\', '/');
									staticMesh.LoadModel(path);
								}
							}
							ImGuiFileDialog::Instance()->Close();
							bShowFileDialog = false;
						}
						ImGui::SetItemDefaultFocus();
						ImGui::SetItemAllowOverlap();
					}

					int materialID = 0;
					for (auto& material : staticMesh.GetMaterials()) {
						std::string label = "Material " + std::to_string(materialID);
						material->OnImGuiRender(label);
						materialID++;
					}
				}

				if (registry.any_of<Engine::CameraComponent>(_SelectedEntity)) {
					ImGui::DragFloat("Camera Speed", &_LevelEditor->GetActiveCamera()->MovementSpeed);
					ImGui::DragFloat("Camera Sens", &_LevelEditor->GetActiveCamera()->MouseSensitivity);
				}

				if (registry.any_of<Engine::BoxColliderComponent>(_SelectedEntity)) {
					auto& box = registry.get<Engine::BoxColliderComponent>(_SelectedEntity);
					ImGui::DragFloat3("Collider Size", glm::value_ptr(box.Size));
					ImGui::Checkbox("Dynamic", &box.IsDynamic);
					if (ImGui::Combo("Collision Enabled", &CollisionEnabled, "Disabled\0Enabled")) {
						box.Mode = static_cast<Engine::ECollisionMode>(CollisionEnabled);
					}
				}

				if (registry.any_of<Engine::RigidBodyComponent>(_SelectedEntity)) {
					auto& rb = registry.get<Engine::RigidBodyComponent >(_SelectedEntity);
					ImGui::DragFloat("Mass", &rb.Mass);
				}

				if (ImGui::Combo("Add Component", &SelectedComboBoxComponentItem, "None\0Static Mesh\0Box Collider\0RigidBody")) {
					switch (SelectedComboBoxComponentItem)
					{
					case 1:
						if (!registry.any_of<EngineStaticMeshComponent>(_SelectedEntity)) {
							registry.emplace<EngineStaticMeshComponent>(_SelectedEntity, "../Game/Assets/Models/Cube/cube.obj");
						}
						SelectedComboBoxComponentItem = 0;
						break;
					case 2:
						if (!registry.any_of<Engine::BoxColliderComponent>(_SelectedEntity)) {
							registry.emplace<Engine::BoxColliderComponent>(_SelectedEntity);
						}
						SelectedComboBoxComponentItem = 0;
						break;
					case 3:
						if (!registry.any_of<Engine::RigidBodyComponent>(_SelectedEntity)) {
							registry.emplace<Engine::RigidBodyComponent>(_SelectedEntity);
							auto& rb = registry.get<Engine::RigidBodyComponent>(_SelectedEntity);
							auto& collider = registry.get<Engine::BoxColliderComponent>(_SelectedEntity);

							EnginePhysicsSystem::Get().AddBoxBody(
								transform.Position,
								collider.Size,
								collider.IsDynamic,
								static_cast<uint32_t>(_SelectedEntity)
							);
						}
						SelectedComboBoxComponentItem = 0;
						break;
					default:
						break;
					}
				}
			}
			ImGui::End();
		}
		ImGui::End();
	}
}