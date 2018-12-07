#include "kdnode.h"

#include <algorithm>

std::unique_ptr<KdNode> KdNode::buildKdNode(std::vector<Triangle>::iterator begin, std::vector<Triangle>::iterator end, int depth) {
	std::unique_ptr<KdNode> node = std::make_unique<KdNode>();
	ptrdiff_t len = end - begin;

	if (len == 1) {
		node->triangle = *begin;
		node->bbox = BoundingBox::fromTriangle(*begin);
	} else if (len > 1) {
		auto mid = begin + len / 2;

		std::nth_element(begin, mid, end, [depth] (const Triangle& t0, const Triangle& t1) {
			return t0.v0->pos[depth] + t0.v1->pos[depth] + t0.v2->pos[depth] < t1.v0->pos[depth] + t1.v1->pos[depth] + t1.v2->pos[depth];
		});

		depth = (depth + 1) % 3;

		node->left = buildKdNode(begin, mid, depth);
		node->right = buildKdNode(mid, end, depth);

		node->bbox = node->left->bbox;
		node->bbox.expand(node->right->bbox);
	}

	return node;
}
