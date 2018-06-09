/*******************************************************************************
 *  DBLoaderModule.cpp
 *
 *  
 *******************************************************************************/

#include "DBModule/include/DBModule.hpp"

DBModule::DBModule(std::string action)
{
	loadImageFiles(action);
    if (action == "LOAD")
		loadObjectList();
	else if (action == "SAVE")
		createDBObjectList();
	else
		LOGEROR("INVALID ACTION TYPE");
}

DBModule::~DBModule()
{

}

void DBModule::loadImageFiles(std::string &_action)
{
    std::string objectListStr = "";
	Config::getConfigFile(Config_directory);
	Config::LoadProfiles();
	Config::loadConfig();
	std::string DBobjectPath;
	DBobjectPath = Config::getString("ObjectRecognition.DBobjPath");
    LOGINFO( "Loading ObjectRecognition.LoadObjects: " <<objectListStr <<"\n");
    std::vector<std::string> objectList;
	boost::filesystem::path p(DBobjectPath);
	boost::filesystem::directory_iterator end_itr;
	int DbObjCount=0, imageCount = 0;
	for (boost::filesystem::directory_iterator itr(p); itr != end_itr; ++itr) {
		if (!is_regular_file(itr->path()))
			continue;
		if (!validateImageFormat(itr->path().string()))
			continue;
		std::string current_image = itr->path().string();// TO DO VALIDATION OF DBOBJ
		std::string archiveFileName = createArchivefiename(current_image);
		if (boost::filesystem::exists(archiveFileName)) {
			ImageListDBMap[current_image] = archiveFileName;
			DbObjCount++;
		}
		else {
			ImageListDBMap[current_image] = "";
			imageCount++;
		}
	}
	if (_action == "LOAD") { 
		if (imageCount)
			LOGINFO("RECREATE DB MODULE SOME IMAGES FILES HAVE NO .objprop ");
	}
	else {
		if (DbObjCount)
			LOGINFO(".objprop Files exixts Kindly delete them and recreate DB");
	}
}

void DBModule::loadObjectList() {
	for (std::map<std::string, std::string>::iterator it = ImageListDBMap.begin(); it != ImageListDBMap.end(); ++it)
	{
		if (it->second.empty()) {
			// TODO CODE FOR DIRECT LODING FROM IMAGE
		}
		else {
			loadObjectProperties(it->second);
		}
	}
}

void DBModule::loadObjectProperties( std::string filename )
{
    if(!boost::filesystem::exists(filename))
    {
        LOGEROR("File not found: " + filename);
        return;
    }
    ObjectProperties* objectProperties;
    try
    {
        LOGINFO( "Loading " + filename);
        std::ifstream input_file( filename, std::ios::binary);
        //boost::archive::text_iarchive ia(ifs);
		boost::archive::binary_iarchive input_archive(input_file);
        objectProperties = new ObjectProperties();
		input_archive >> objectProperties;
		input_file.close();
    }
	catch (std::exception & e)
    {
        LOGEROR( "Cannot load object: " << e.what() );
        return;
    }
	LOGINFO(" GOING FOR Testing Load OBject ");
    //m_ORMatchingModule->addObjectProperties(objectProperties);
	objectProperties->testLoadedObjects();
    delete objectProperties;
}


void DBModule::createDBObjectList() {
	for (std::map<std::string, std::string>::iterator it = ImageListDBMap.begin(); it != ImageListDBMap.end(); ++it)
	{
		if (it->second.empty()) {
			createDBojects(it->first);
		}
		else {
			// TODO NEEDS TO DECIDE WHAT TO DO FOR EXISTING DB OBJECTS
		}
	}
}

void DBModule::createDBojects(std::string _filename) {
	try
	{
		ObjectProperties* objectProperties= new ObjectProperties(_filename);
		objectProperties->featchObjectProperties();		
		std::string archiveFileName = createArchivefiename(_filename);
		std::ofstream output_fileName(archiveFileName, std::ios::binary);
		//ost::archive::text_oarchive ia(ofs);
		boost::archive::binary_oarchive output_archive(output_fileName);
		output_archive << objectProperties ;
		output_fileName.close();
	}
	catch (std::exception & e)
	{
		LOGEROR("Cannot load object: " << e.what());
		return;
	}
}

inline bool DBModule::validateImageFormat(const std::string &fileName) {
	std::string extension = boost::filesystem::extension(fileName);
	for (int i = 0; i < Img_formats.size(); ++i) {
		if (fileName.length() > Img_formats[i].length() &&
			Img_formats[i].compare(extension) == 0)
			return true;
		else
			continue;
	}
	return false;
}

inline std::string DBModule::createArchivefiename(const std::string &fileName) {
	return boost::filesystem::change_extension(fileName, "dat").string();
}
