// ObjectSfM - Object Based Structure-from-Motion.
// Copyright (C) 2018  Ohio State University, CEGE, GDA group
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef OBJECTSFM_OPTIMIZER_REPROJECTION_ERROR_POSE_XYZ_H_
#define OBJECTSFM_OPTIMIZER_REPROJECTION_ERROR_POSE_XYZ_H_

#include "ceres/ceres.h"
#include "ceres/rotation.h"

namespace objectsfm
{
	// pose: [0,1,2] are the angle-axis, [3,4,5] are the translation t.
	// xyz: is the x, y, z coordinates of the 3D point
	struct ReprojectionErrorPoseXYZ
	{
		ReprojectionErrorPoseXYZ(const double observed_x, const double observed_y, const double* cam, const double weight)
			: observed_x(observed_x), observed_y(observed_y), cam(cam), weight(weight) {}

		template <typename T>
		bool operator()(const T* const pose,
			const T* const xyz,
			T* residuals) const
		{
			// pose[0,1,2] are the angle-axis rotation.
			T p[3];
			ceres::AngleAxisRotatePoint(pose, xyz, p);

			// pose[3,4,5] are the translation.
			p[0] += pose[3];
			p[1] += pose[4];
			p[2] += pose[5];

			// Compute the center of distortion. The sign change comes from
			// the camera model that Noah Snavely's Bundler assumes, whereby
			// the camera coordinate system has a negative z axis.
			const T xp = p[0] / p[2];
			const T yp = p[1] / p[2];

			// Apply second and fourth order radial distortion.
			const T& focal = T(cam[0]);
			const T& l1 = T(cam[1]);
			const T& l2 = T(cam[2]);
			const T r2 = xp*xp + yp*yp;
			const T distortion = 1.0 + r2  * (l1 + l2  * r2);

			// Compute final projected point position.
			const T predicted_x = focal * distortion * xp;
			const T predicted_y = focal * distortion * yp;

			// The error is the difference between the predicted and observed position.
			residuals[0] = weight*(predicted_x - observed_x);
			residuals[1] = weight*(predicted_y - observed_y);
			
			return true;
		}

		// Factory to hide the construction of the CostFunction object from
		// the client code.
		static ceres::CostFunction* Create(const double observed_x, const double observed_y, const double *cam, const double weight)
		{
			return (new ceres::AutoDiffCostFunction<ReprojectionErrorPoseXYZ, 2, 6, 3>(
				new ReprojectionErrorPoseXYZ(observed_x, observed_y, cam, weight)));
		}

		const double observed_x;
		const double observed_y;
		const double *cam;
		const double weight;
	};

}  // namespace objectsfm

#endif  // OBJECTSFM_OPTIMIZER_REPROJECTION_ERROR_POSE_XYZ_H_
