#include "editor_node.h"
#include <imgui.h>
#include "imnodes.h"

void EditorNode::ShowEditorNode()
{
	ImGui::Begin("node editor");
    {
        ImNodes::BeginNodeEditor();
        ImNodes::BeginNode(1);

        ImNodes::BeginNodeTitleBar();
        ImGui::TextUnformatted("simple node :)");
        ImNodes::EndNodeTitleBar();

        ImNodes::BeginInputAttribute(2);
        ImGui::Text("input");
        ImNodes::EndInputAttribute();

        ImNodes::BeginOutputAttribute(3);
        ImGui::Indent(40);
        ImGui::Text("output");
        ImNodes::EndOutputAttribute();

        ImNodes::EndNode();
        ImNodes::EndNodeEditor();
    }
    ImGui::End();
}