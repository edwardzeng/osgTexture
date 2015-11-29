/**********************************************************
*Write by FlySky
*zzuxp@163.com  http://www.OsgChina.org   
**********************************************************/

#include <osgViewer/Viewer>

#include <osg/Node>
#include <osg/Geode>
#include <osg/Group>
#include <osg/Image>
#include <osg/TexGen>
#include <osg/Texture1D>
#include <osg/TexEnv>
#include <osg/StateSet>

#include <osgDB/ReadFile>
#include <osgDB/WriteFile>

#include <osgUtil/Optimizer>

//创建一维纹理属性
osg::StateSet* createTexture1DState()
{
	//创建贴图对象，实践上是一个高度为1的二维图像
	osg::Image* image = new osg::Image;

	//为image分配一个空间
	image->allocateImage(1024,1,1,GL_RGBA,GL_FLOAT);
	//设置纹理图像数据格式RGBA
	image->setInternalTextureFormat(GL_RGBA);

	//为image填充数据
	osg::Vec4* dataPtr = (osg::Vec4*)image->data();
	for(int i=0;i<1024;++i)
	{
		*dataPtr++ = osg::Vec4(1.0f,0.5f,0.8f,0.5f);
	}

	//创建一维纹理
	osg::Texture1D* texture = new osg::Texture1D();
	//设置环绕模式
	texture->setWrap(osg::Texture1D::WRAP_S,osg::Texture1D::MIRROR);
	//设置滤波
	texture->setFilter(osg::Texture1D::MIN_FILTER,osg::Texture1D::LINEAR);
	//设置贴图
	texture->setImage(image);

	//设置自动纹理坐标，并指定相关的平面
	osg::TexGen* texgen = new osg::TexGen;
	texgen->setMode(osg::TexGen::OBJECT_LINEAR);
	texgen->setPlane(osg::TexGen::S,osg::Plane(0.0f,0.0f,1.0f,-10000));

	//创建属性集
	osg::StateSet* stateset = new osg::StateSet;

	//启用一维纹理
	stateset->setTextureAttribute(0,texture,osg::StateAttribute::OVERRIDE);
	stateset->setTextureMode(0,GL_TEXTURE_1D,osg::StateAttribute::ON|osg::StateAttribute::OVERRIDE);

	//启用纹理生成器
	stateset->setTextureAttribute(0,texgen,osg::StateAttribute::OVERRIDE);
	stateset->setTextureMode(0,GL_TEXTURE_GEN_S,osg::StateAttribute::ON|osg::StateAttribute::OVERRIDE);

	return stateset;
}

int main()
{
	osgViewer::Viewer* viewer = new osgViewer::Viewer();

	osg::Group* root = new osg::Group();

	//读取模型
	osg::Node* node = new osg::Node() ;
	node = osgDB::readNodeFile("F:/3rdpart/OSG/OpenSceneGraph-3.0.1-VS10.0.30319-x64/data/cessna.osg");

	//使用一维纹理属性
	osg::StateSet* stateset = new osg::StateSet();
	stateset =createTexture1DState();

	node->setStateSet(stateset);
	root->addChild(node);

	//优化场景数据
	osgUtil::Optimizer optimizer ;
	optimizer.optimize(root) ;

	viewer->setSceneData(root);

	viewer->realize();

	viewer->run();

	return 0 ;
}