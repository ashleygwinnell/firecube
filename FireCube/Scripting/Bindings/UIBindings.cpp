#include "lua.hpp"
#include "sol.hpp"
#include "Scripting/LuaBindings.h"
#include "UI/UI.h"
#include "UI/UIText.h"
#include "UI/UIImage.h"
#include "Rendering/Font.h"
#include "Rendering/Texture2D.h"

using namespace FireCube;

UIElement *NewElement(Engine *engine)
{
	return new UIElement(engine);
}

sol::object CreateChild(UIElement *element, const std::string &type, sol::this_state s)
{
	if (type == "UIElement")
	{
		auto child = element->CreateChild<UIElement>();
		return sol::object(s, sol::in_place, child);
	}
	else if (type == "UIText")
	{
		auto child = element->CreateChild<UIText>();
		return sol::object(s, sol::in_place, child);
	}
	else if (type == "UIImage")
	{
		auto child = element->CreateChild<UIImage>();
		return sol::object(s, sol::in_place, child);
	}
	else
	{
		return sol::make_object(s, sol::nil);
	}
}

void LuaBindings::InitUI(sol::state &luaState)
{
	luaState.new_usertype<UI>("UI",
		"root", sol::property(&UI::GetRoot),
		sol::base_classes, sol::bases<Object, RefCounted>());

	luaState.new_usertype<UIElement>("UIElement",
		"new", &NewElement,
		"parent", sol::property(&UIElement::GetParent, &UIElement::SetParent),
		"position", sol::property(&UIElement::GetPosition, &UIElement::SetPosition),
		"screenPosition", sol::property(&UIElement::GetScreenPosition),
		"CreateChild", &CreateChild,
		"AddChild", &UIElement::AddChild,
		"Remove", &UIElement::Remove,
		"DelayRemove", &UIElement::DelayRemove,
		"RemoveAllChildren", &UIElement::RemoveAllChildren,
		"DelayRemoveAllChildren", &UIElement::DelayRemoveAllChildren,
		sol::base_classes, sol::bases<Object, RefCounted>());

	luaState.new_usertype<UIText>("UIText",
		"fontFace", sol::property(&UIText::GetFontFace, &UIText::SetFontFace),
		"text", sol::property(&UIText::GetText, &UIText::SetText),
		"color", sol::property(&UIText::GetColor, &UIText::SetColor),
		sol::base_classes, sol::bases<UIElement, Object, RefCounted>());

	luaState.new_usertype<UIImage>("UIImage",
		"image", sol::property(&UIImage::GetImage, &UIImage::SetImage),
		"size", sol::property(&UIImage::GetSize, &UIImage::SetSize),
		"color", sol::property(&UIImage::GetColor, &UIImage::SetColor),
		sol::base_classes, sol::bases<UIElement, Object, RefCounted>());
}