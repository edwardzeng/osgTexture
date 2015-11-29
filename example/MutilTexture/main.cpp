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

	//��ȡ��ͼ
	osg::Image* image = osgDB::readImageFile("E:/3rdpart/OSG/OpenSceneGraph-3.0.1-VS10.0.30319-x64/data/Images/primitives.gif");

	if (image)
	{
		//������ά����
		osg::Texture2D* texture = new osg::Texture2D;
		texture->setImage(image);

		//�����Զ���������
		osg::TexGen* texgen = new osg::TexGen;
		texgen->setMode(osg::TexGen::SPHERE_MAP);

		//������������ģʽΪBLEND��
		osg::TexEnv* texenv = new osg::TexEnv;
		texenv->setMode(osg::TexEnv::BLEND);
		//����BLEND��������ɫ
		texenv->setColor(osg::Vec4(0.6f,0.6f,0.6f,0.0f));

		//���õ�Ԫ1�Զ������������꣬��ʹ������
		osg::StateSet* stateset = new osg::StateSet;
		stateset->setTextureAttributeAndModes(1,texture,osg::StateAttribute::ON);
		stateset->setTextureAttributeAndModes(1,texgen,osg::StateAttribute::ON);

		//����������
		stateset->setTextureAttribute(1,texenv);

		//��������״̬
		node->setStateSet(stateset);
	}

	root->addChild(node);

	//�Ż���������
	osgUtil::Optimizer optimizer ;
	optimizer.optimize(root) ;

	viewer->setSceneData(root);

	viewer->realize();

	viewer->run();

	return 0 ;
}