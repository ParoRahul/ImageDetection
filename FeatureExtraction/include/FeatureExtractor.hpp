#ifndef FEATUREEXTRACTOR_H
#define FEATUREEXTRACTOR_H

#include <dlib/pixel.h>
#include <dlib/array2d.h>
#include <dlib/image_loader/image_loader.h>
#include <dlib/image_keypoint.h>
#include <dlib/data_io.h>
#include <dlib/image_processing.h>
#include <dlib/image_io.h>
#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d.hpp>
#include <boost/filesystem.hpp>
#include "ProjectProperty/include/ProjectProperty.hpp"
#include "ProjectProperty/include/Config.hpp"

class FeatureExtractor{
      public :
      FeatureExtractor( const Extractor_Type & _extractor_type, const std::string & _fileName );
      void extractKeyPoint(std::vector<cv::KeyPoint> & _keyPointList, cv::Mat & _descriptor);	      
      //void extractDescriptor(std::vector<cv::KeyPoint> & _keyPointList, cv::Mat & _descriptor);	      
      private :
      Extractor_Type extractor_type;
      std::string fileName;
      Image_Type getImageFormat(const std::string & _fileName );
};
#endif

