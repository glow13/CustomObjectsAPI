#pragma once
#include <utility>

namespace rectpack2D {
	using total_area_type = float;

	struct rect_wh {
		rect_wh() : w(0), h(0) {}
		rect_wh(const float w, const float h) : w(w), h(h) {}

		float w;
		float h;

		auto& flip() {
			std::swap(w, h);
			return *this;
		}

		float max_side() const {
			return h > w ? h : w;
		}

		float min_side() const {
			return h < w ? h : w;
		}

		float area() const { return w * h; }
	   	float perimeter() const { return 2 * w + 2 * h; }

		template <class R>
		void expand_with(const R& r) {
			w = std::max(w, r.x + r.w);
			h = std::max(h, r.y + r.h);
		}
	};

	struct rect_xywh {
		float x;
		float y;
		float w;
		float h;

		rect_xywh() : x(0), y(0), w(0), h(0) {}
		rect_xywh(const float x, const float y, const float w, const float h) : x(x), y(y), w(w), h(h) {}

		float area() const { return w * h; }
		float perimeter() const { return 2 * w + 2 * h; }

		auto get_wh() const {
			return rect_wh(w, h);
		}
	};

	struct rect_xywhf {
		float x;
		float y;
		float w;
		float h;
		bool flipped;

		rect_xywhf() : x(0), y(0), w(0), h(0), flipped(false) {}
		rect_xywhf(const float x, const float y, const float w, const float h, const bool flipped) : x(x), y(y), w(flipped ? h : w), h(flipped ? w : h), flipped(flipped) {}
		rect_xywhf(const rect_xywh& b) : rect_xywhf(b.x, b.y, b.w, b.h, false) {}

		float area() const { return w * h; }
		float perimeter() const { return 2 * w + 2 * h; }

		auto get_wh() const {
			return rect_wh(w, h);
		}
	};

	using space_rect = rect_xywh;
}