/**********************************************************
*Write by FlySky
*zzuxp@163.com  http://www.OsgChina.org   
**********************************************************/

#include <osgViewer/Viewer>

#include <osg/Vec3>
#include <osg/Vec4>
#include <osg/Quat>
#include <osg/Matrix>
#include <osg/ShapeDrawable>
#include <osg/Geometry>
#include <osg/Geode>
#include <osg/Transform>
#include <osg/Material>
#include <osg/NodeCallback>
#include <osg/Depth>
#include <osg/CullFace>
#include <osg/TexMat>
#include <osg/TexGen>
#include <osg/TexEnv>
#include <osg/TextureCubeMap>

#include <osgDB/WriteFile>
#include <osgDB/ReadFile>

#include <osgUtil/Optimizer>

#include <iostream>

//读取立方图
osg::TextureCubeMap* readCubeMap()
{
	osg::TextureCubeMap* cubemap = new osg::TextureCubeMap;

	osg::Image* imagePosX = osgDB::readImageFile("E:/3rdpart/OSG/OpenSceneGraph-3.0.1-VS10.0.30319-x64/data/right.jpg");
	osg::Image* imageNegX = osgDB::readImageFile("E:/3rdpart/OSG/OpenSceneGraph-3.0.1-VS10.0.30319-x64/data/left.jpg");
	osg::Image* imagePosY = osgDB::readImageFile("E:/3rdpart/OSG/OpenSceneGraph-3.0.1-VS10.0.30319-x64/data/front.jpg");
	osg::Image* imageNegY = osgDB::readImageFile("E:/3rdpart/OSG/OpenSceneGraph-3.0.1-VS10.0.30319-x64/data/back.jpg");
	osg::Image* imagePosZ = osgDB::readImageFile("E:/3rdpart/OSG/OpenSceneGraph-3.0.1-VS10.0.30319-x64/data/up.jpg");
	osg::Image* imageNegZ = osgDB::readImageFile("E:/3rdpart/OSG/OpenSceneGraph-3.0.1-VS10.0.30319-x64/data/down.jpg");

	if (imagePosX && imageNegX && imagePosY && imageNegY && imagePosZ && imageNegZ)
	{
		//设置立方图的六个面的贴图
		cubemap->setImage(osg::TextureCubeMap::POSITIVE_X, imagePosX);
		cubemap->setImage(osg::TextureCubeMap::NEGATIVE_X, imageNegX);
		cubemap->setImage(osg::TextureCubeMap::POSITIVE_Y, imagePosY);
		cubemap->setImage(osg::TextureCubeMap::NEGATIVE_Y, imageNegY);
		cubemap->setImage(osg::TextureCubeMap::POSITIVE_Z, imagePosZ);
		cubemap->setImage(osg::TextureCubeMap::NEGATIVE_Z, imageNegZ);

		//设置纹理环绕模式
		cubemap->setWrap(osg::Texture::WRAP_S, osg::Texture::CLAMP_TO_EDGE);
		cubemap->setWrap(osg::Texture::WRAP_T, osg::Texture::CLAMP_TO_EDGE);
		cubemap->setWrap(osg::Texture::WRAP_R, osg::Texture::CLAMP_TO_EDGE);

		//设置滤波：线形和mipmap
		cubemap->setFilter(osg::Texture::MIN_FILTER, osg::Texture::LINEAR_MIPMAP_LINEAR);
		cubemap->setFilter(osg::Texture::MAG_FILTER, osg::Texture::LINEAR);
	}

	return cubemap;
}

//更新立方体图纹理
struct TexMatCallback : public osg::NodeCallback
{
public:

	TexMatCallback(osg::TexMat& tm) :
	  _texMat(tm)
	  {
		  //
	  }

	  virtual void operator()(osg::Node* node, osg::NodeVisitor* nv)
	  {
		  osgUtil::CullVisitor* cv = dynamic_cast<osgUtil::CullVisitor*>(nv);
		  if (cv)
		  {
			  //得到模型视图矩阵并设置旋转角度
			  const osg::Matrix& MV = *(cv->getModelViewMatrix());
			  const osg::Matrix R = osg::Matrix::rotate( osg::DegreesToRadians(112.0f), 0.0f,0.0f,1.0f)*
				  osg::Matrix::rotate( osg::DegreesToRadians(90.0f), 1.0f,0.0f,0.0f);

			  osg::Quat q = MV.getRotate();
			  const osg::Matrix C = osg::Matrix::rotate( q.inverse() );

			  //设置纹理矩阵
			  _texMat.setMatrix( C*R );
		  }

		  traverse(node,nv);
	  }

	  //纹理矩阵
	  osg::TexMat& _texMat;
};

//一个变换类，使天空盒绕视点旋转
class MoveEarthySkyWithEyePointTransform : public osg::Transform
{
public:
	//局部矩阵计算成世界矩阵
	virtual bool computeLocalToWorldMatrix(osg::Matrix& matrix,osg::NodeVisitor* nv) const 
	{
		osgUtil::CullVisitor* cv = dynamic_cast<osgUtil::CullVisitor*>(nv);
		if (cv)
		{
			osg::Vec3 eyePointLocal = cv->getEyeLocal();
			matrix.preMult(osg::Matrix::translate(eyePointLocal));
		}
		return true;
	}

	//世界矩阵计算为局部矩阵
	virtual bool computeWorldToLocalMatrix(osg::Matrix& matrix,osg::NodeVisitor* nv) const
	{
		osgUtil::CullVisitor* cv = dynamic_cast<osgUtil::CullVisitor*>(nv);
		if (cv)
		{
			osg::Vec3 eyePointLocal = cv->getEyeLocal();
			matrix.postMult(osg::Matrix::translate(-eyePointLocal));
		}
		return true;
	}
};

//创建天空盒
osg::Node* createSkyBox()
{
	osg::StateSet* stateset = new osg::StateSet();

	//设置纹理映射方式，指定为替代方式，即纹理中的颜色代替原来的颜色
	osg::TexEnv* te = new osg::TexEnv;
	te->setMode(osg::TexEnv::REPLACE);
	stateset->setTextureAttributeAndModes(0, te, osg::StateAttribute::ON| osg::StateAttribute::OVERRIDE);

	//自动生成纹理坐标，反射方式(REFLECTION_MAP)
	/*
	NORMAL_MAP　标准模式－立方图纹理
	REFLECTION_MAP　反射模式－球体纹理
	SPHERE_MAP　球体模型－球体纹理
	*/
	osg::TexGen *tg = new osg::TexGen;
	tg->setMode(osg::TexGen::NORMAL_MAP);
	stateset->setTextureAttributeAndModes(0, tg, osg::StateAttribute::ON| osg::StateAttribute::OVERRIDE);

	//设置纹理矩阵
	osg::TexMat *tm = new osg::TexMat;
	stateset->setTextureAttribute(0, tm);

	//设置立方图纹理
	osg::TextureCubeMap* skymap = readCubeMap();
	stateset->setTextureAttributeAndModes(0, skymap, osg::StateAttribute::ON| osg::StateAttribute::OVERRIDE);

	stateset->setMode( GL_LIGHTING, osg::StateAttribute::OFF );
	stateset->setMode( GL_CULL_FACE, osg::StateAttribute::OFF );

	//将深度设置为远平面
	osg::Depth* depth = new osg::Depth;
	depth->setFunction(osg::Depth::ALWAYS);
	depth->setRange(1.0,1.0);//远平面   
	stateset->setAttributeAndModes(depth, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);

	//设置渲染顺序为-1，先渲染
	stateset->setRenderBinDetails(-1,"RenderBin");

	osg::Drawable* drawable = new osg::ShapeDrawable(new osg::Sphere(osg::Vec3(0.0f,0.0f,0.0f),1));

	//把球体加入到叶节点
	osg::Geode* geode = new osg::Geode;
	geode->setCullingActive(false);
	geode->setStateSet( stateset );
	geode->addDrawable(drawable);

	//设置变换
	osg::Transform* transform = new MoveEarthySkyWithEyePointTransform();
	transform->setCullingActive(false);
	transform->addChild(geode);

	osg::ClearNode* clearNode = new osg::ClearNode;
	clearNode->setCullCallback(new TexMatCallback(*tm));
	clearNode->addChild(transform);

	return clearNode;
}

int main()
{
	osgViewer::Viewer* viewer= new osgViewer::Viewer();

	osg::Group* rootnode = new osg::Group();

	//加入天空盒
	rootnode->addChild(createSkyBox());;

	//优化场景数据
	osgUtil::Optimizer optimzer;
	optimzer.optimize(rootnode);

	viewer->setSceneData(rootnode);

	viewer->realize();

	viewer->run();

	return 0 ;
}
