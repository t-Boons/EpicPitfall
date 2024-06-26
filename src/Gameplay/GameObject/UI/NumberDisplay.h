#pragma once

namespace Pitfall
{
	class NumberDisplay : public GameObject
	{
	public:
		NumberDisplay(const String& name, const int2& position);

		void SetValue(int number) { m_Score = number; m_NumRenderer.SetNumber(m_Score); }

		virtual void Render() override;
		virtual bool ShouldRender() const override { return true; }

	private:
		NumberRenderer m_NumRenderer;
		int m_Score;
	};
}