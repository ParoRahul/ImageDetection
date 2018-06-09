#include "FeatureExtraction/include/FeatureExtractor.hpp"


FeatureExtractor::FeatureExtractor( const Extractor_Type & _extractor_type, const std::string & _fileName )
	:extractor_type(_extractor_type),fileName(_fileName){
	
	}

void FeatureExtractor::extractKeyPoint(std::vector<cv::KeyPoint> & _keyPointList,cv::Mat & _descriptor){
        if(!boost::filesystem::exists(fileName))
           {
               LOGEROR("File not found: " + fileName);
               return;
           }
        switch( extractor_type)
        {
        case SURF_DLIB:
	{
             dlib::array2d<dlib::rgb_pixel> img;
             //Image_Type Imgtype = getImageFormat(fileName);
	     double detection_threshold = Config::getFloat("surf.DetectionThreshold");
	     int max_points = Config::getInt("surf.NumPoints");
	     int desc_size = Config::getInt("surf.DescriptorLength");
	     dlib::load_image(img, fileName.c_str()); 
             std::vector<dlib::surf_point> surfpoint = dlib::get_surf_points(img, max_points, detection_threshold);
	     for (auto & sp : surfpoint ){
                  cv::KeyPoint kp;
                  kp.pt.x = sp.p.center(0);
                  kp.pt.y = sp.p.center(1);
                  kp.size = sp.p.scale;
                  kp.angle = sp.angle;
                  kp.response = sp.p.score;
                  cv::Mat Desc(1,desc_size,CV_64FC1);
                  for(int j=0; j<desc_size; j++){
                      Desc.at<float>(0,j) = sp.des(j,1);
                  }
                  //memcpy(Desc.data, &panoKeyPoints[i]._vec, matSize);
		  _keyPointList.push_back(kp);
                  _descriptor.push_back(Desc);
	     }
	     LOGINFO(" TOTAL NUMBER KEYPOINT EXTRACTED " + std::to_string(surfpoint.size()));
             break;
	}
        case SIFT_VLFET:
	{
             LOGINFO(" CODING FOR SIFT  NOT DONE ");
             break;
	}
        case INVALID:
	{
             LOGEROR(" INVALID FeatureExtractor  ");
             break;
	}
        default: 
                  LOGEROR(" INVALID FeatureExtractor  ");
        }
}

Image_Type FeatureExtractor::getImageFormat(const std::string & _fileName ){
        std::string extension = boost::filesystem::extension(_fileName);
        Image_Type img_type;
        if (extension == "JPG" || extension == "jpg" || extension == "jpeg" ||  extension == "jpe" )
            img_type = Image_Type::JPG;
        else if ( extension == "png" || extension == "PNG" )
            img_type = Image_Type::PNG;
        else
            img_type = Image_Type::INVALID;
        return img_type;
}
