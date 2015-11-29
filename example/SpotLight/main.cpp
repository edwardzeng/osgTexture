/**********************************************************
*Write by FlySky
*zzuxp@163.com  http://www.OsgChina.org   
**********************************************************/

#include <osgViewer/Viewer>

#include <osg/Node>
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/Group>
#include <osg/Camera>
#include <osg/MatrixTransform>
#include <osg/PositionAttitudeTransform>
#include <osg/Light>
#include <osg/LightSource>
#include <osg/TexGen>
#include <osg/ShapeDrawable>
#include <osg/Texture2D>
#include <osg/TexGenNode>

#include <osgDB/ReadFile>
#include <osgDB/WriteFile>

#include <osgUtil/Optimizer>

#include <iostream>

#include "Tex.h"

//�����۹�������mipmap��ͼ
osg::Image* createSpotLightImage(const osg::Vec4& centerColour, const osg::Vec4& backgroudColour, unsigned int size, float power)
{
	//����Image����
	osg::Image* image = new osg::Image;
	//��̬����һ��size*size��С��image
	image->allocateImage(size,size,1,GL_RGBA,GL_UNSIGNED_BYTE);

	//���image
	//������Ϊԭ�㣬��ɫ��������˥��
	float mid = (float(size)-1)*0.5f;
	float div = 2.0f/float(size);
	for(unsigned int r=0;r<size;++r)
	{
		unsigned char* ptr = image->data(0,r,0);
		for(unsigned int c=0;c<size;++c)
		{
			float dx = (float(c) - mid)*div;
			float dy = (float(r) - mid)*div;
			float r = powf(1.0f-sqrtf(dx*dx+dy*dy),power);
			if (r<0.0f) r=0.0f;
			osg::Vec4 color = centerColour*r+backgroudColour*(1.0f-r);
			*ptr++ = (unsigned char)((color[0])*255.0f);
			*ptr++ = (unsigned char)((color[1])*255.0f);
			*ptr++ = (unsigned char)((color[2])*255.0f);
			*ptr++ = (unsigned char)((color[3])*255.0f);
		}
	}
	return image;
}

//�����۹��״̬����
osg::StateSet* createSpotLightDecoratorState(unsigned int lightNum, unsigned int textureUnit)
{
	osg::StateSet* stateset = new osg::StateSet;

	//����IDΪlightNum�Ĺ���
	stateset->setMode(GL_LIGHT0+lightNum, osg::StateAttribute::ON);

	//�������ĵ���ɫ�ͻ��������ɫ
	osg::Vec4 centerColour(1.0f,1.0f,1.0f,1.0f);
	osg::Vec4 ambientColour(0.05f,0.05f,0.05f,1.0f); 

	//�����۹������
	osg::Texture2D* texture = new osg::Texture2D();
	texture->setImage(createSpotLightImage(centerColour, ambientColour, 64, 1.0));
	texture->setBorderColor(osg::Vec4(ambientColour));
	texture->setWrap(osg::Texture::WRAP_S,osg::Texture::CLAMP_TO_BORDER);
	texture->setWrap(osg::Texture::WRAP_T,osg::Texture::CLAMP_TO_BORDER);
	texture->setWrap(osg::Texture::WRAP_R,osg::Texture::CLAMP_TO_BORDER);

	//������Ԫ
	stateset->setTextureAttributeAndModes(textureUnit, texture, osg::StateAttribute::ON);

	//�����Զ�������������
	stateset->setTextureMode(textureUnit, GL_TEXTURE_GEN_S, osg::StateAttribute::ON);
	stateset->setTextureMode(textureUnit, GL_TEXTURE_GEN_T, osg::StateAttribute::ON);
	stateset->setTextureMode(textureUnit, GL_TEXTURE_GEN_R, osg::StateAttribute::ON);
	stateset->setTextureMode(textureUnit, GL_TEXTURE_GEN_Q, osg::StateAttribute::ON);

	return stateset;
}

//�����۹�ƽڵ�
osg::Node* createSpotLightNode(const osg::Vec3& position, const osg::Vec3& direction, float angle, unsigned int lightNum, unsigned int textureUnit)
{
	osg::Group* group = new osg::Group;

	//������Դ
	osg::LightSource* lightsource = new osg::LightSource;
	osg::Light* light = lightsource->getLight();
	light->setLightNum(lightNum);
	light->setPosition(osg::Vec4(position,1.0f));
	light->setAmbient(osg::Vec4(0.00f,0.00f,0.05f,1.0f));
	light->setDiffuse(osg::Vec4(1.0f,1.0f,1.0f,1.0f));
	group->addChild(lightsource);

	//���㷨����
	osg::Vec3 up(0.0f,0.0f,1.0f);
	up = (direction ^ up) ^ direction;
	up.normalize();

	//�����Զ�������������ڵ�
	osg::TexGenNode* texgenNode = new osg::TexGenNode;
	//��������Ԫ
	texgenNode->setTextureUnit(textureUnit);
	//������������������
	osg::TexGen* texgen = texgenNode->getTexGen();
	//����ģʽΪ�Ӿ�����
	texgen->setMode(osg::TexGen::EYE_LINEAR);
	//����ͼ��ָ���ο�ƽ��
	texgen->setPlanesFromMatrix(osg::Matrixd::lookAt(position, position+direction, up)*
		osg::Matrixd::perspective(angle,1.0,0.1,100));

	group->addChild(texgenNode);

	return group;
}

//��������·��(��ο������½ڵ�OSG����)
osg::AnimationPath* createAnimationPath(const osg::Vec3& center,float radius,double looptime)
{
	osg::AnimationPath* animationPath = new osg::AnimationPath;
	animationPath->setLoopMode(osg::AnimationPath::LOOP);

	int numSamples = 40;
	float yaw = 0.0f;
	float yaw_delta = 2.0f*osg::PI/((float)numSamples-1.0f);
	float roll = osg::inDegrees(30.0f);

	double time=0.0f;
	double time_delta = looptime/(double)numSamples;
	for(int i=0;i<numSamples;++i)
	{
		osg::Vec3 position(center+osg::Vec3(sinf(yaw)*radius,cosf(yaw)*radius,0.0f));
		osg::Quat rotation(osg::Quat(roll,osg::Vec3(0.0,1.0,0.0))*osg::Quat(-(yaw+osg::inDegrees(90.0f)),osg::Vec3(0.0,0.0,1.0)));

		animationPath->insert(time,osg::AnimationPath::ControlPoint(position,rotation));

		yaw += yaw_delta;
		time += time_delta;

	}
	return animationPath;    
}

//��������ƽ��
osg::Node* createBase(const osg::Vec3& center,float radius)
{
	osg::Geode* geode = new osg::Geode;

	osg::StateSet* stateset = new osg::StateSet();
	osg::Image* image = osgDB::readImageFile("E:/3rdpart/OSG/OpenSceneGraph-3.0.1-VS10.0.30319-x64/data/Images/lz.rgb");
	if (image)
	{
		osg::Texture2D* texture = new osg::Texture2D;
		texture->setImage(image);
		stateset->setTextureAttributeAndModes(0,texture,osg::StateAttribute::ON);
	}

	geode->setStateSet( stateset );

	osg::HeightField* grid = new osg::HeightField;
	grid->allocate(38,39);
	grid->setOrigin(center+osg::Vec3(-radius,-radius,0.0f));
	grid->setXInterval(radius*2.0f/(float)(38-1));
	grid->setYInterval(radius*2.0f/(float)(39-1));

	float minHeight = FLT_MAX;
	float maxHeight = -FLT_MAX;


	unsigned int r;
	for(r=0;r<39;++r)
	{
		for(unsigned int c=0;c<38;++c)
		{
			float h = vertex[r+c*39][2];
			if (h>maxHeight) maxHeight=h;
			if (h<minHeight) minHeight=h;
		}
	}

	float hieghtScale = radius*0.5f/(maxHeight-minHeight);
	float hieghtOffset = -(minHeight+maxHeight)*0.5f;

	for(r=0;r<39;++r)
	{
		for(unsigned int c=0;c<38;++c)
		{
			float h = vertex[r+c*39][2];
			grid->setHeight(c,r,(h+hieghtOffset)*hieghtScale);
		}
	}

	geode->addDrawable(new osg::ShapeDrawable(grid));

	osg::Group* group = new osg::Group;
	group->addChild(geode);

	return group;

}

//��������ģ��
osg::Node* createMovingModel(const osg::Vec3& center, float radius)
{
	float animationLength = 10.0f;

	osg::AnimationPath* animationPath = createAnimationPath(center,radius,animationLength);

	osg::Group* model = new osg::Group;

	osg::Node* cessna = osgDB::readNodeFile("E:/3rdpart/OSG/OpenSceneGraph-3.0.1-VS10.0.30319-x64/data/cessna.osg");
	if (cessna)
	{
		const osg::BoundingSphere& bs = cessna->getBound();

		float size = radius/bs.radius()*0.3f;
		osg::MatrixTransform* positioned = new osg::MatrixTransform;
		positioned->setDataVariance(osg::Object::STATIC);
		positioned->setMatrix(osg::Matrix::translate(-bs.center())*
			osg::Matrix::scale(size,size,size)*
			osg::Matrix::rotate(osg::inDegrees(180.0f),0.0f,0.0f,2.0f));

		positioned->addChild(cessna);

		osg::MatrixTransform* xform = new osg::MatrixTransform;
		xform->setUpdateCallback(new osg::AnimationPathCallback(animationPath,0.0f,2.0));
		xform->addChild(positioned);

		//��Ӿ۹�ƽڵ�
		xform->addChild(createSpotLightNode(osg::Vec3(0.0f,0.0f,0.0f), osg::Vec3(0.0f,1.0f,-1.0f), 60.0f, 0, 1));

		model->addChild(xform);
	}

	return model;
}

//��������
osg::Node* createModel()
{
	osg::Vec3 center(0.0f,0.0f,0.0f);
	float radius = 100.0f;

	//��������ģ��
	osg::Node* shadower = createMovingModel(center,radius*0.5f);

	//��������ƽ��
	osg::Node* shadowed = createBase(center-osg::Vec3(0.0f,0.0f,radius*0.1),radius);

	//����������ڵ�
	osg::Group* root = new osg::Group;

	//����״̬����
	root->setStateSet(createSpotLightDecoratorState(0,1));

	//����ӽڵ�
	root->addChild(shadower);
	root->addChild(shadowed);

	return root;
}

int main()
{
	osgViewer::Viewer* viewer = new osgViewer::Viewer();

	osg::Group* root = new osg::Group();

	//��ӳ���
	root->addChild(createModel());

	//�Ż���������
	osgUtil::Optimizer optimizer ;
	optimizer.optimize(root) ;

	viewer->setSceneData(root);

	viewer->realize();

	viewer->run();

	return 0 ;
}