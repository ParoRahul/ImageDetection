#include "ProjectProperty/include/Config.hpp"
#include <iostream>
#include <fstream>

boost::mutex Config::Mutex;

Config* Config::m_inst=0;

std::map<std::string, ConfigEntry > Config::m_values ;

std::string Config::DirectoryName = " ";

std::vector<std::string> Config::profilesToLoad;

Config::Config()
{
  
}

Config::~Config()
{
  
}

Config* Config::getInstance()
{
  boost::mutex::scoped_lock lock(Mutex);
  if ( !m_inst )
  {
	  if (m_values.empty()) {
		  LOGINFO("No instance was set! Trying to load 'default' profile from file 'Config.xml'..");
		  const std::string DName = Config_directory;
		  if (DName.empty())
			  getConfigFile(DName);
		  if (profilesToLoad.empty())
			  LoadProfiles();
		  Config* newConf = new Config();
		  newConf->loadFromFile();
		  setInstance(newConf);
	  }
	  else {
		  Config* newConf = new Config();
		  setInstance(newConf);
	  }
  }
  return m_inst;
}

void  Config::setInstance(Config* newInst)
{
	Config* oldInst = m_inst;
	m_inst = newInst;
	delete oldInst;
}

void Config::getConfigFile(const std::string _directoryName) {
	DirectoryName = _directoryName;
	std::vector< std::string > fileNames;
	boost::filesystem::path p(_directoryName);
	boost::filesystem::directory_iterator end_itr;
	for (boost::filesystem::directory_iterator itr(p); itr != end_itr; ++itr) {
		if (is_regular_file(itr->path())) {
			std::string current_Xml = itr->path().string();
			if (itr->path().filename().string().compare("Merged.xml") == 0)
				continue;
			else
			    fileNames.push_back(current_Xml);
		}
	}
	std::ofstream outStream;
	std::string mergedFile = _directoryName + "/Merged.xml";
	outStream.open(mergedFile.c_str());
	outStream << "<?xml version=\"1.0\" standalone=\"no\" ?>\n\n";
	for (unsigned i = 0; i<fileNames.size(); i++)
	{
		LOGINFO("Loading " + fileNames[i]);
		std::string line;
		std::ifstream configFile(fileNames[i].c_str());
		if (configFile.is_open()) {
			while (configFile.good())
			{
				getline(configFile, line);
				outStream << line << std::endl;
			}
			configFile.close();
		}
	}
	outStream.close();
}

void Config::LoadProfiles() {
	std::string mergedFile = DirectoryName + "/Merged.xml";
	boost::property_tree::ptree pt;
	read_xml(mergedFile, pt);
	BOOST_FOREACH(boost::property_tree::ptree::value_type const& v, pt.get_child("")) {
		profilesToLoad.push_back(v.first);
	}
}

void Config::loadConfig()
{
    Config* newConf=new Config();
	if (!newConf->loadFromFile())
	{
		LOGEROR("Could not load config file!");
		exit(-126);
	}
    Config::setInstance(newConf);
}

bool Config::loadFromFile()
{
	std::string mergedFile = DirectoryName + "/Merged.xml";
	boost::property_tree::ptree tree;
	read_xml(mergedFile, tree);
	for (std::string profiles : profilesToLoad) {
		BOOST_FOREACH(boost::property_tree::ptree::value_type const& v, tree.get_child(profiles))
		{
			if (v.first == "<xmlcomment>")
				continue;
			if (v.first != "<xmlattr>")
			{
				ConfigEntry newEntry;
				if (v.second.get_child("<xmlattr>.type").data() == "INTEGER") {
					int tmp = stoi(v.second.get_child("<xmlattr>.value").data());
					newEntry.setInt(tmp);
					newEntry.setIntMin(tmp);
					newEntry.setIntMax(tmp);
				}
				else if (v.second.get_child("<xmlattr>.type").data() == "FLOAT") {
					float tmp = stof(v.second.get_child("<xmlattr>.value").data());
					newEntry.setFloat(tmp);
					newEntry.setFloatMin(tmp);
					newEntry.setFloatMax(tmp);
				}
				else if (v.second.get_child("<xmlattr>.type").data() == "STRING") {
					const char *temp = v.second.get_child("<xmlattr>.value").data().c_str();
					newEntry.setString(temp);
				}
				else if (v.second.get_child("<xmlattr>.type").data() == "BOOLEAN") {
					int temp = stoi(v.second.get_child("<xmlattr>.value").data());
					newEntry.setInt(temp);
				}
				else {
					continue;
				}
				m_values[profiles + "." + v.second.get_child("<xmlattr>.name").data()] = newEntry;
			}
		}
	}
	std::cout << "Total Number of Property Loaded " << m_values.size() << std::endl;
	boost::filesystem::remove_all(mergedFile);
	return true;
}

int Config::getInt(const std::string& id)
{
	return getInstance()->get<int>(id);
}

bool Config::getBool(const std::string& id)
{
	return getInstance()->get<int>(id);
}

std::string Config::getString(const std::string& id)
{
	return getInstance()->get<const char*>(id);
}

float Config::getFloat(const std::string& id)
{
	return getInstance()->get<float>(id);
}


bool Config::getFloatMinMax(const std::string& id, std::pair<float, float>& minmax)
{
	return getInstance()->getMinMax<float>(id, minmax);
}

bool Config::getIntMinMax(const std::string& id, std::pair<int, int>& minmax)
{
	return getInstance()->getMinMax<int>(id, minmax);
}

void Config::clear()
{
	boost::mutex::scoped_lock lock(Mutex);
	m_values.clear();
	profilesToLoad.clear();
}