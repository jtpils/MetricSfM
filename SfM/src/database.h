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

#ifndef OBJECTSFM_DATABASE_H_
#define OBJECTSFM_DATABASE_H_
 
#include <string>  
#include <vector>  

#include <opencv2/opencv.hpp>

#include "basic_structs.h"

#include "fbow/vocabulary_creator.h"
#include "fbow/fbow.h"

namespace objectsfm {

class Database
{
public:
	Database();
	~Database();

	DatabaseOptions options;

	bool FeatureExtraction();

	/* SFM */

	int num_imgs_;

	std::string input_fold_, output_fold_;

	// default formats: jpg, png, bmp, tiff, JPG
	std::vector<std::string> image_formats_;

	std::vector<std::string> image_paths_;

	std::vector<ImageInfo*> image_infos_;

	std::vector<ListKeyPoint*> keypoints_;

	std::vector<cv::Mat*> descriptors_;

	std::vector<std::vector<float>> gist_descriptors_;

	void SearchImagePaths();

	bool CheckFeatureIndexExist();

	std::vector<int> CheckMissingFeatureFile();

	bool ExtractImageInfo(int idx);

	bool LoadAllImageInfo();

	void ExtractImageFeatures(int idx);

	bool ReadinImageFeatures(int idx);	

	bool ReadinImageKeyPoints(int idx);

	bool WriteoutImageFeature(int idx);

	bool WriteoutImageFeature(int idx, std::string path);

	void ReleaseImageFeatures(int idx);

	bool WriteoutGistFeature();

	bool ReadinGistFeature();

	/* Vocabulary */

	fbow::VocabularyCreator voc_creator_;

	fbow::Vocabulary voc_;

	void BuildVocabularyTree();

	void ReadinVocabularyTree();

	void WriteoutVocabularyTree();

	bool CheckVocabularyTreeExist();

	/* Words*/
	std::vector<fbow::fBow*> words_fbow_;

	std::vector<std::map<uint32_t,float>*> words_vector_;

	std::vector<std::vector<int>> words_id_;
	
	int max_words_id_;

	void BuildWords();

	bool CheckWordsIndexExist();

	std::vector<int> CheckMissingWordsFile();

	void GenerateWordsForImage(int idx);

	void ReadinWordsForImage(int idx);

	void WriteoutWordsForImage(int idx);

	void WriteoutWordsForImage(int idx, std::string path);

	void ReleaseWordsForImage(int idx);

	/* satellite image*/
	std::string fold_dsm_, zone_id_;

	int dsm_rows_, dsm_cols_, bands_, ellipsoid_id_;

	double ori_x_, ori_y_, gsd_x_, gsd_y_;

	float *img_data_;

	cv::Mat img_cropped_, edge_map_, non_building_mask_;

	int super_pixel_size_;

	float img_pixel_scale_;

	std::vector<cv::Point3f> site_pts_;

	std::vector<std::vector<cv::Point3f>> site_visible_pts_;

	std::vector<std::vector<float>> site_pts_angles_;

	std::vector<std::vector<float>> site_visible_lines_;

	void ReadinDSMInfo();

	void ReadinDSMImage();

	void ReadinDSMImage(float lat, float lon, float radius);

	void ExtractSites();

	void ExtractSiteVisiblePts();

	void ExtractSiteVisibleLines();

	void WriteoutVisibleLines();

	void ReadinVisibleLines();

};

}  // namespace objectsfm

#endif  // OBJECTSFM_OBJ_DATABASE_H_