#include "epch.h"
#include "Material.h"
#include "imgui.h"
#include "ImGuiFileDialog.h"

namespace Engine {

	void Material::AddTexture(ETextureType type, std::shared_ptr<Texture2D> texture)
	{
		_Textures[type] = texture;
	}

	std::shared_ptr<Texture2D> Material::GetTexture(ETextureType type) const
	{
		auto it = _Textures.find(type);
		if (it != _Textures.end())
			return it->second;
		return NULL;
	}

	bool bShowFileDialog = false;
	ETextureType currentTextureType;
	void Material::OnImGuiRender(const std::string& name /*= "Material"*/)
	{
		if (ImGui::TreeNode(name.c_str())) {
			ImGui::Checkbox("Highlight", &_bHighlight);
			for (const auto& [type, texture] : _Textures) {
				std::string label;
				switch (type) {
				case ETextureType::Diffuse:  label = "diffuse";  break;
				case ETextureType::Specular: label = "specular"; break;
				case ETextureType::Normal:   label = "normal";   break;
				case ETextureType::AmbientOcclusion: label = "ambient";   break;
				}

				ImGui::Text("%s", label.c_str());
				if (ImGui::ImageButton((void*)(intptr_t)texture->GetTextureID(), ImVec2(64, 64))) {
					bShowFileDialog = !bShowFileDialog;
					currentTextureType = type;
				}
				if (ImGui::IsItemHovered() && texture) {
					ImGui::BeginTooltip();
					ImGui::Text("Path: %s", texture->GetFile().c_str());
					ImGui::EndTooltip();
				}

				if (bShowFileDialog) {
					IGFD::FileDialogConfig config;

					ImGuiFileDialog::Instance()->OpenDialog("ChooseTexture", "Choose Texture", ".png,.jpg,.jpeg,.bmp,.tga", config);

					if (ImGuiFileDialog::Instance()->Display("ChooseTexture")) {
						if (ImGuiFileDialog::Instance()->IsOk()) {
							std::string path = ImGuiFileDialog::Instance()->GetFilePathName();
							if (!path.empty()) {
								AddTexture(currentTextureType, Texture2D::Create(path, "texture_" + label));
							}
						}
						ImGuiFileDialog::Instance()->Close();
						bShowFileDialog = false;
					}
					ImGui::SetItemDefaultFocus();
					ImGui::SetItemAllowOverlap();
				}

				if (ImGui::Button("Reset Texture")) {
					AddTexture(type, Texture2D::Create("", label));
				}
			}
			ImGui::DragFloat("Metalness", &_metalness);
			ImGui::DragFloat("Roughness", &_roughness);
			ImGui::DragFloat("Shininess", &_shininess);
			ImGui::TreePop();
		}
	}

}