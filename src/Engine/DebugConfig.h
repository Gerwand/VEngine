#pragma once

namespace vengine {

/* Simple structure containing information for debug drawing */
struct DebugConfig {
	bool drawOctree;	/* Draw octree nosed bounding areas */
	bool drawColliders; /* Draw AABB colliders */
	bool drawPositions; /* Draw centers of the objects */
};

}