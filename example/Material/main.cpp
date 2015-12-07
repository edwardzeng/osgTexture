#include <osgViewer/Viewer>
#include <osg/Node>
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/Group>
#include <osg/StateSet>
#include <osg/Material>
#include <osg/CullFace>
#include <osgDB/ReadFile>
#include <osgDB/WriteFile>
#include <osgUtil/Optimizer>
#include <osg/StateAttribute>
#include <iostream>

enum MaterialColor{
	RED,
	GREEN,
	BLUE
};

void setMaterialColor(osg::Node* node,MaterialColor materialcolor)
{
	osg::Geometry* geom;
	//get state attribute 
	osg::StateSet* stateset = new osg::StateSet();
	stateset= node->getOrCreateStateSet() ;
	
	//create material object
	osg::Material* mat = new osg::Material();
	mat->setAmbient(osg::Material::FRONT_AND_BACK,osg::Vec4(0.2f,0.2f,0.2f,1.0f));
	//set front diffuse color
	mat->setDiffuse(osg::Material::FRONT_AND_BACK,osg::Vec4(0.3f,0.8f,0.3f,1.0f));
	//set front specular color
	mat->setSpecular(osg::Material::FRONT_AND_BACK,osg::Vec4(0.2f,0.6f,0.2f,1.0f));

	mat->setShininess(osg::Material::FRONT_AND_BACK,28.0f);

	//mat->setColorMode(osg::Material::AMBIENT_AND_DIFFUSE);
	stateset->setAttribute(mat);
}

int main()
{
	osgViewer::Viewer* viewer = new osgViewer::Viewer();
	osg::Group* root = new osg::Group();
	osg::ref_ptr<osg::Node> node = new osg::Node();
	node = osgDB::readNodeFile("E:/SourceCode/OSG/OSGData/Robot/arm1.ive");
	setMaterialColor(node.get(),BLUE);
	root->addChild(node);
	osgUtil::Optimizer optimizer ;
	optimizer.optimize(root) ;
	viewer->setSceneData(root);
	viewer->realize();
	viewer->run();
	return 0 ;
}