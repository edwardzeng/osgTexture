/**********************************************************
*Write by FlySky
*zzuxp@163.com  http://www.OsgChina.org   
**********************************************************/


#include <osgViewer/Viewer>

#include <osg/Node>
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/Group>
#include <osg/Texture2D>
#include <osg/TexGen>
#include <osg/TexEnv>

#include <osgDB/ReadFile>
#include <osgDB/WriteFile>

#include <osgUtil/Optimizer>

int main()
{
	osgViewer::Viewer* viewer = new osgViewer::Viewer();

	osg::Group* root = new osg::Group();

	osg::Node* node = new osg::Node();
	node= osgDB::readNodeFile("E:\\3rdpart\\OSG\\OpenSceneGraph-3.0.1-VS10.0.30319-x64\\data\\cow.osg");

	//读取贴图
	osg::Image* image = osgDB::readImageFile("E:/3rdpart/OSG/OpenSceneGraph-3.0.1-VS10.0.30319-x64/data/Images/primitives.gif");

	if (image)
	{
		//创建二维纹理
		osg::Texture2D* texture = new osg::Texture2D;
		texture->setImage(image);

		//设置自动生成坐标
		osg::TexGen* texgen = new osg::TexGen;
		texgen->setMode(osg::TexGen::SPHERE_MAP);

		//设置纹理环境，模式为BLEND，
		osg::TexEnv* texenv = new osg::TexEnv;
		texenv->setMode(osg::TexEnv::BLEND);
		//设置BLEND操作的颜色
		texenv->setColor(osg::Vec4(0.6f,0.6f,0.6f,0.0f));

		//启用单元1自动生成纹理坐标，并使用纹理
		osg::StateSet* stateset = new osg::StateSet;
		stateset->setTextureAttributeAndModes(1,texture,osg::StateAttribute::ON);
		stateset->setTextureAttributeAndModes(1,texgen,osg::StateAttribute::ON);

		//设置纹理环境
		stateset->setTextureAttribute(1,texenv);

		//设置纹理状态
		node->setStateSet(stateset);
	}

	root->addChild(node);

	//优化场景数据
	osgUtil::Optimizer optimizer ;
	optimizer.optimize(root) ;

	viewer->setSceneData(root);

	viewer->realize();

	viewer->run();

	return 0 ;
}