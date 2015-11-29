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

//����һά��������
osg::StateSet* createTexture1DState()
{
	//������ͼ����ʵ������һ���߶�Ϊ1�Ķ�άͼ��
	osg::Image* image = new osg::Image;

	//Ϊimage����һ���ռ�
	image->allocateImage(1024,1,1,GL_RGBA,GL_FLOAT);
	//��������ͼ�����ݸ�ʽRGBA
	image->setInternalTextureFormat(GL_RGBA);

	//Ϊimage�������
	osg::Vec4* dataPtr = (osg::Vec4*)image->data();
	for(int i=0;i<1024;++i)
	{
		*dataPtr++ = osg::Vec4(1.0f,0.5f,0.8f,0.5f);
	}

	//����һά����
	osg::Texture1D* texture = new osg::Texture1D();
	//���û���ģʽ
	texture->setWrap(osg::Texture1D::WRAP_S,osg::Texture1D::MIRROR);
	//�����˲�
	texture->setFilter(osg::Texture1D::MIN_FILTER,osg::Texture1D::LINEAR);
	//������ͼ
	texture->setImage(image);

	//�����Զ��������꣬��ָ����ص�ƽ��
	osg::TexGen* texgen = new osg::TexGen;
	texgen->setMode(osg::TexGen::OBJECT_LINEAR);
	texgen->setPlane(osg::TexGen::S,osg::Plane(0.0f,0.0f,1.0f,-10000));

	//�������Լ�
	osg::StateSet* stateset = new osg::StateSet;

	//����һά����
	stateset->setTextureAttribute(0,texture,osg::StateAttribute::OVERRIDE);
	stateset->setTextureMode(0,GL_TEXTURE_1D,osg::StateAttribute::ON|osg::StateAttribute::OVERRIDE);

	//��������������
	stateset->setTextureAttribute(0,texgen,osg::StateAttribute::OVERRIDE);
	stateset->setTextureMode(0,GL_TEXTURE_GEN_S,osg::StateAttribute::ON|osg::StateAttribute::OVERRIDE);

	return stateset;
}

int main()
{
	osgViewer::Viewer* viewer = new osgViewer::Viewer();

	osg::Group* root = new osg::Group();

	//��ȡģ��
	osg::Node* node = new osg::Node() ;
	node = osgDB::readNodeFile("F:/3rdpart/OSG/OpenSceneGraph-3.0.1-VS10.0.30319-x64/data/cessna.osg");

	//ʹ��һά��������
	osg::StateSet* stateset = new osg::StateSet();
	stateset =createTexture1DState();

	node->setStateSet(stateset);
	root->addChild(node);

	//�Ż���������
	osgUtil::Optimizer optimizer ;
	optimizer.optimize(root) ;

	viewer->setSceneData(root);

	viewer->realize();

	viewer->run();

	return 0 ;
}