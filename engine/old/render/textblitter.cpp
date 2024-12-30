//
// Created by Administrator on 5/21/2024.
//

#include "textblitter.h"

#include <constants.hpp>


namespace backend::renderapi::opengl {
	class Texture;
}

int _xMargin = 0;
	int _yMargin = 0;
	int _xDebugMargin = 0;
	int _yDebugMargin = 4;
	int _lineHeight = 16;
	int _charSpacing = 0;
	int _spaceWidth = 6;
	std::string _charSheet = "•!\"#$%&\'••*+,-./0123456789:;<=>?_ABCDEFGHIJKLMNOPQRSTUVWXYZ\\^_`abcdefghijklmnopqrstuvwxyz";
//	std::string _numSheet = "1234567890/";
	std::string _textToBilt = "";
	int _charCursorIndex = 0;
	float _textTime = 0;
	float _textSpeed = 200.0f;
	float _countdownTimer = 0;
	float _delayTimer = 0;

	struct BlitXY {
		std::string text;
		int x = 0;
		int y = 0;
		bool centered = false;
		float scale = 1.0f;
	};
	std::vector<BlitXY> blitXYs;

	void TextBlitter::Type(std::string text, float coolDownTimer, float delayTimer) {
		ResetBlitter();
		_textToBilt = text;

		if (coolDownTimer == -1)
			_countdownTimer = 2.0;
		else
			_countdownTimer = coolDownTimer;

		if (delayTimer == -1)
			_delayTimer = 0;
		else
			_delayTimer = delayTimer;
	}

	void TextBlitter::AddDebugText(std::string text) {
		_debugTextToBilt += text + "\n";
	}

	void TextBlitter::ResetDebugText() {
		_debugTextToBilt = "";
	}

	void TextBlitter::ResetBlitter() {
		_textTime = 0;
		_textToBilt = "";
		_charCursorIndex = 0;
		_countdownTimer = 0;
	}

	void TextBlitter::Update(float deltaTime) {

		//_objectData.clear();

		// Decrement timer
		if (_delayTimer > 0) {
			_delayTimer -= deltaTime;

			if (_delayTimer < 0) {
				//	Audio::PlayAudio("RE_type.wav");
			}
		}
		// increment text time
		else
			_textTime += (_textSpeed * deltaTime);

		_charCursorIndex = (int)_textTime;

		float xcursor = (float)_xMargin;
		float ycursor = (float)_yMargin;
		//int color = 0; // 0 for white, 1 for green
		glm::vec3 color = WHITE;


		// Decrement timer
		if (_countdownTimer > 0)
			_countdownTimer -= deltaTime;
		// Wipe the text if timer equals zero
		if (_countdownTimer < 0) {
			ResetBlitter();
		}

		if (_delayTimer <= 0) {
			// Type blitting
			for (int i = 0; i < _textToBilt.length() && i < _charCursorIndex; i++)
			{
				char character = _textToBilt[i];
				if (_textToBilt[i] == '[' &&
					_textToBilt[(size_t)i + 1] == 'w' &&
					_textToBilt[(size_t)i + 2] == ']') {
					i += 2;
					color = WHITE;
					continue;
				}
				if (_textToBilt[i] == '[' &&
					_textToBilt[(size_t)i + 1] == 'g' &&
					_textToBilt[(size_t)i + 2] == ']') {
					i += 2;
					color = GREEN;
					continue;
				}
				if (character == ' ') {
					xcursor += _spaceWidth;
					continue;
				}
				if (character == '\n') {
					xcursor = _xMargin;
					ycursor += _lineHeight;
					continue;
				}

				size_t charPos = _charSheet.find(character);
				if (charPos == std::string::npos)
					continue;
				backend::renderapi::opengl::Texture* texture = AssetManager::GetTextureByName("char_" + std::to_string(charPos + 1));

				float texWidth = (float)texture->GetWidth();
				float texHeight = (float)texture->GetHeight();
				float cursor_X = xcursor + texWidth;
				float cursor_Y = ycursor + texHeight - _lineHeight;

				UIRenderInfo renderInfo;
				renderInfo.centered = false;
				renderInfo.textureName = "char_" + std::to_string(charPos + 1);
				renderInfo.screenX = (int)(cursor_X - texWidth);
				renderInfo.screenY = cursor_Y;
                Renderer_OLD::QueueUIForRendering(renderInfo);

				xcursor += texWidth + _charSpacing;
			}
		}

		// Debug text
		color = WHITE;
		xcursor = _xDebugMargin;
		ycursor = _yDebugMargin;
		for (int i = 0; i < _debugTextToBilt.length(); i++)
		{
			char character = _debugTextToBilt[i];
			if (_debugTextToBilt[i] == '[' &&
				_debugTextToBilt[(size_t)i + 1] == 'w' &&
				_debugTextToBilt[(size_t)i + 2] == ']') {
				i += 2;
				color = WHITE;
				continue;
			}
			if (_debugTextToBilt[i] == '[' &&
				_debugTextToBilt[(size_t)i + 1] == 'g' &&
				_debugTextToBilt[(size_t)i + 2] == ']') {
				i += 2;
				color = GREEN;
				continue;
			}
			if (character == ' ') {
				xcursor += _spaceWidth;
				continue;
			}
			if (character == '\n') {
				xcursor = _xDebugMargin;
				ycursor += _lineHeight;
				continue;
			}
			size_t charPos = _charSheet.find(character);
			if (charPos == std::string::npos)
				continue;
			Texture* texture = AssetManager::GetTextureByName("char_" + std::to_string(charPos + 1));

			float texWidth = (float)texture->GetWidth();
			float texHeight = (float)texture->GetHeight();
			float cursor_X = xcursor + texWidth;
			float cursor_Y = ycursor + texHeight - _lineHeight;

			UIRenderInfo renderInfo;
			renderInfo.centered = false;
			renderInfo.textureName = "char_" + std::to_string(charPos + 1);
			renderInfo.screenX = cursor_X - texWidth;
			renderInfo.screenY = cursor_Y;
            Renderer_OLD::QueueUIForRendering(renderInfo);

			xcursor += texWidth + _charSpacing;
		}

		// BlitXY
		for (auto& blitXY : blitXYs) {

			// Find center (NOTE THIS DOES NOT WORK FOR MULTI LINE)
			if (blitXY.centered) {
				xcursor = 0;
				for (int i = 0; i < blitXY.text.length(); i++) {
					char character = blitXY.text[i];
					if (blitXY.text[i] == '[' &&
						blitXY.text[(size_t)i + 1] == 'w' &&
						blitXY.text[(size_t)i + 2] == ']') {
						i += 2;
						color = WHITE;
						continue;
					}
					if (blitXY.text[i] == '[' &&
						blitXY.text[(size_t)i + 1] == 'g' &&
						blitXY.text[(size_t)i + 2] == ']') {
						i += 2;
						color = GREEN;
						continue;
					}
					if (character == ' ') {
						xcursor += _spaceWidth;
						continue;
					}
					if (character == '\n') {
						xcursor = blitXY.x;
						ycursor -= _lineHeight;
						continue;
					}
					size_t charPos = _charSheet.find(character);
                    Texture* texture = AssetManager::GetTextureByName("char_" + std::to_string(charPos + 1));
                    float texWidth = (float)texture->GetWidth();
                    float texHeight = (float)texture->GetHeight();
					xcursor += texWidth + _charSpacing;
				}
				blitXY.x -= (int)(xcursor / 2);
			}

			color = WHITE;
			xcursor = blitXY.x;
			ycursor = blitXY.y;

			// Blit
			for (int i = 0; i < blitXY.text.length(); i++)
			{
				char character = blitXY.text[i];
				if (blitXY.text[i] == '[' &&
					blitXY.text[(size_t)i + 1] == 'w' &&
					blitXY.text[(size_t)i + 2] == ']') {
					i += 2;
					color = WHITE;
					continue;
				}
				if (blitXY.text[i] == '[' &&
					blitXY.text[(size_t)i + 1] == 'g' &&
					blitXY.text[(size_t)i + 2] == ']') {
					i += 2;
					color = GREEN;
					continue;
				}
				if (character == ' ') {
					xcursor += _spaceWidth;
					continue;
				}
				if (character == '\n') {
					xcursor = blitXY.x;
					ycursor += _lineHeight;
					continue;
				}
				size_t charPos = _charSheet.find(character);
				Texture* texture = AssetManager::GetTextureByName("char_" + std::to_string(charPos + 1));

				float texWidth = (float)texture->GetWidth();
				float texHeight = (float)texture->GetHeight();
				float cursor_X = xcursor + texWidth;
				float cursor_Y = ycursor + texHeight - _lineHeight;

				UIRenderInfo renderInfo;
				renderInfo.centered = false;
				renderInfo.textureName = "char_" + std::to_string(charPos + 1);
				renderInfo.screenX = (int)(cursor_X - texWidth);
				renderInfo.screenY = (int)cursor_Y;
				renderInfo.color = color;
                Renderer_OLD::QueueUIForRendering(renderInfo);

				xcursor += texWidth + _charSpacing;
			}
		}
		_debugTextToBilt = "";
	}

	void TextBlitter::BlitAtPosition(std::string text, int x, int y, bool centered, float scale)
	{
		BlitXY blitXY;
		blitXY.text = text;
		blitXY.x = x;
		blitXY.y = y;
		blitXY.centered = centered;
		blitXY.scale = scale;
		blitXYs.push_back(blitXY);
	}
}

int TextBlitter::GetTextWidth(const std::string& text) {
	float maxLineWidth = 0;
	float xcursor = 0;
	for (int i = 0; i < text.length(); i++) {
		char character = text[i];
		if (text[i] == '[' &&
			text[(size_t)i + 1] == 'w' &&
			text[(size_t)i + 2] == ']') {
			i += 2;
			continue;
		}
		if (text[i] == '[' &&
			text[(size_t)i + 1] == 'g' &&
			text[(size_t)i + 2] == ']') {
			i += 2;
			continue;
		}
		if (character == ' ') {
			xcursor += _spaceWidth;
			continue;
		}
		if (character == '\n') {
			xcursor = 0;
			continue;
		}
		size_t charPos = _charSheet.find(character);
		Texture* texture = AssetManager::GetTextureByName("char_" + std::to_string(charPos + 1));
		float texWidth = (float)texture->GetWidth();
		xcursor += texWidth + _charSpacing;
		maxLineWidth = std::max(maxLineWidth, xcursor);
	}
	return maxLineWidth;
}

int TextBlitter::GetLineHeight() {
	return _lineHeight;
}

void TextBlitter::ClearAllText() {
    blitXYs.clear();
}

namespace TextBlitter {

    std::vector<RenderItem2D> _renderItems;

    void CreateRenderItems(float renderTargetWidth, float renderTargetHeight) {

        _renderItems.clear();

        // Debug text
        float yDebugMargin = renderTargetHeight - _lineHeight;
        glm::vec3 color = WHITE;
        int xcursor = _xDebugMargin;
        int ycursor = yDebugMargin;
        for (int i = 0; i < _debugTextToBilt.length(); i++)
        {
            char character = _debugTextToBilt[i];
            if (_debugTextToBilt[i] == '[' &&
                _debugTextToBilt[(size_t)i + 1] == 'w' &&
                _debugTextToBilt[(size_t)i + 2] == ']') {
                i += 2;
                color = WHITE;
                continue;
            }
            if (_debugTextToBilt[i] == '[' &&
                _debugTextToBilt[(size_t)i + 1] == 'g' &&
                _debugTextToBilt[(size_t)i + 2] == ']') {
                i += 2;
                color = GREEN;
                continue;
            }
            if (_debugTextToBilt[i] == '[' &&
                _debugTextToBilt[(size_t)i + 1] == 'g' &&
                _debugTextToBilt[(size_t)i + 2] == ']') {
                i += 2;
                color = RED;
                continue;
            }
            if (character == ' ') {
                xcursor += _spaceWidth;
                continue;
            }
            if (character == '\n') {
                xcursor = _xDebugMargin;
                ycursor -= _lineHeight;
                continue;
            }
            size_t charPos = _charSheet.find(character);
            if (charPos == std::string::npos) {
                continue;
            }

            std::string textureName = "char_" + std::to_string(charPos + 1);

            // Get texture index and dimensions
            int textureIndex = AssetManager::GetTextureIndexByName(textureName);
            Texture* texture = AssetManager::GetTextureByIndex(textureIndex);
            float texWidth = texture->GetWidth();
            float texHeight = texture->GetHeight();
            float width = (1.0f / renderTargetWidth) * texWidth;
            float height = (1.0f / renderTargetHeight) * texHeight;
            float cursor_X = ((xcursor + (texWidth / 2.0f)) / renderTargetWidth) * 2 - 1;
            float cursor_Y = ((ycursor + (texHeight / 2.0f)) / renderTargetHeight) * 2 - 1;

            Transform transform;
            transform.position.x = cursor_X;
            transform.position.y = cursor_Y;
            transform.scale = glm::vec3(width, height * -1, 1);

            if (BackEnd::GetAPI() == API::VULKAN) {
                transform.position.y *= -1;
            }

            RenderItem2D renderItem;
            renderItem.modelMatrix = transform.to_mat4();
            renderItem.textureIndex = textureIndex;
            renderItem.colorTintR = color.r;
            renderItem.colorTintG = color.g;
            renderItem.colorTintB = color.b;
            _renderItems.push_back(renderItem);

            xcursor += texWidth + _charSpacing;
        }
        _debugTextToBilt = "";
    }

    std::vector<RenderItem2D>& GetRenderItems() {
        return _renderItems;
    }

}