#include "mvDragPayload.h"
#include "mvInput.h"
#include "mvApp.h"
#include "mvItemRegistry.h"
#include "mvImGuiThemeScope.h"
#include "mvFontScope.h"

namespace Marvel {

	void mvDragPayload::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{
		mvPythonParser parser(mvPyDataType::String, "Undocumented function", { "Containers", "Widgets" });
		mvAppItem::AddCommonArgs(parser, (CommonParserArgs)(
			MV_PARSER_ARG_ID |
			MV_PARSER_ARG_PARENT |
			MV_PARSER_ARG_SHOW)
		);

		parser.addArg<mvPyDataType::String>("payload_type", mvArgType::KEYWORD_ARG, "'$$DPG_PAYLOAD'");

		parser.finalize();

		parsers->insert({ s_command, parser });
	}

	mvDragPayload::mvDragPayload(const std::string& name)
		: mvAppItem(name)
	{
	}

	void mvDragPayload::draw(ImDrawList* drawlist, float x, float y)
	{
		mvImGuiThemeScope scope(this);
		mvFontScope fscope(this);

		if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
		{
			static int preview = 7;
			ImGui::SetDragDropPayload(m_payloadType.c_str(), &preview, sizeof(int));

			if(m_parentPtr->getDragCallback())
				mvApp::GetApp()->getCallbackRegistry().addCallback(m_parentPtr->getDragCallback(), m_parent, m_parentPtr->getDragData());

			for (auto& childset : m_children)
			{
				for (auto& item : childset)
				{

					if (!item->preDraw())
						continue;

					item->draw(drawlist, ImGui::GetCursorPosX(), ImGui::GetCursorPosY());

					item->postDraw();
				}
			}

			ImGui::EndDragDropSource();
		}
	}

	void mvDragPayload::handleSpecificKeywordArgs(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		if (PyObject* item = PyDict_GetItemString(dict, "payload_type")) m_payloadType = ToString(item);
	}

	void mvDragPayload::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;
	}

}