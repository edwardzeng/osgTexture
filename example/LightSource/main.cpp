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
#include <osg/Light>
#include <osg/LightSource>
#include <osg/BoundingSphere>
#include <osg/BoundingBox>
#include <osg/ShapeDrawable>
#include <osg/MatrixTransform>
#include <osg/PositionAttitudeTransform>

#include <osgDB/ReadFile>
#include <osgDB/WriteFile>

#include <osgUtil/Optimizer>

//�򳡾�����ӹ�Դ
osg::Group* createLight(osg::Node* node)
{
	osg::Group* lightRoot= new osg::Group();
	lightRoot->addChild(node);

	//��������
	osg::StateSet* stateset = new osg::StateSet();
	stateset = lightRoot->getOrCreateStateSet();

	stateset->setMode(GL_LIGHTING,osg::StateAttribute::ON);
	stateset->setMode(GL_LIGHT1,osg::StateAttribute::ON);
	//stateset->setMode(GL_LIGHT1,osg::StateAttribute::ON);
	//stateset->setMode(GL_LIGHT2,osg::StateAttribute::ON);

	//�����Χ��
	osg::BoundingSphere bs ;
	node->computeBound() ;
	bs=node->getBound() ;

	//����һ��Light����
	osg::Light* light = new osg::Light();
	light->setLightNum(1);
	//���÷���
	light->setDirection(osg::Vec3(0.0f,0.0f,-1.0f));
	//����λ��
	light->setPosition(osg::Vec4(bs.center().x(),bs.center().y(),bs.center().z()+bs.radius(),1.0f));
	//���û��������ɫ
	light->setAmbient(osg::Vec4(0.6f,0.6f,0.6f,1.0f));
	//����ɢ������ɫ
	light->setDiffuse(osg::Vec4(1.2f,1.2f,1.2f,1.0f));

	//���ú�˥��ָ��
	light->setConstantAttenuation(1.0f);
	//��������˥��ָ��
	light->setLinearAttenuation(0.1f);
	//���ö��η�˥��ָ��
	//light->setQuadraticAttenuation(0.01f);

	//������Դ
	osg::LightSource* lightSource = new osg::LightSource();
	lightSource->setLight(light);
	//lightSource->setLocalStateSetModes(osg::StateAttribute::ON);
	//lightSource->setStateSetModes(*stateset,osg::StateAttribute::ON);

	lightRoot->addChild(lightSource);

	osg::ref_ptr<osg::Geode> sphere =new osg::Geode;
	sphere->addDrawable(new osg::ShapeDrawable(new osg::Sphere(osg::Vec3(0.0f,0.0f,0.0f),0.4)));
	osg::ref_ptr<osg::MatrixTransform> trans=new osg::MatrixTransform;
	trans->setMatrix(osg::Matrix::translate(bs.center().x(),bs.center().y(),bs.center().z()+bs.radius()));
	trans->addChild(sphere);

	lightRoot->addChild(trans);
	return lightRoot ;
}

osg::Group* createLight1(osg::Node* node)
{
	osg::Group* lightRoot= new osg::Group();
	lightRoot->addChild(node);

	//��������
	osg::StateSet* stateset = new osg::StateSet();
	stateset = lightRoot->getOrCreateStateSet();

	//stateset->setMode(GL_LIGHTING,osg::StateAttribute::ON);
	//stateset->setMode(GL_LIGHT1,osg::StateAttribute::ON);
	//stateset->setMode(GL_LIGHT1,osg::StateAttribute::ON);
	//stateset->setMode(GL_LIGHT2,osg::StateAttribute::ON);

	//����һ��Light����
	osg::Light* light = new osg::Light();
	light->setLightNum(1);
	//���÷���
	light->setDirection(osg::Vec3(0.0f,0.0f,-1.0f));
	//����λ��
	light->setPosition(osg::Vec4(-6.4f,-1.96f,16.0f,1.0f));

	osg::ref_ptr<osg::Geode> sphere =new osg::Geode;
	sphere->addDrawable(new osg::ShapeDrawable(new osg::Sphere(osg::Vec3(0.0f,0.0f,0.0f),0.5)));
	osg::ref_ptr<osg::PositionAttitudeTransform> position=new osg::PositionAttitudeTransform;
	position->setPosition(osg::Vec3(-6.4f,-1.96f,16.0f));
	position->addChild(sphere);

	//���û��������ɫ
	light->setAmbient(osg::Vec4(0.2f,0.2f,0.2f,1.0f));
	//����ɢ������ɫ
	light->setDiffuse(osg::Vec4(1.0f,0.0f,0.0f,1.0f));

	//���ú�˥��ָ��
	light->setConstantAttenuation(1.0f);
	//��������˥��ָ��
	light->setLinearAttenuation(0.0f);
	//���ö��η�˥��ָ��
	//light->setQuadraticAttenuation(0.01f);

	//������Դ
	osg::LightSource* lightSource = new osg::LightSource();
	lightSource->setLight(light);
	lightSource->setLocalStateSetModes(osg::StateAttribute::ON);
	lightSource->setStateSetModes(*stateset,osg::StateAttribute::ON);

	lightRoot->addChild(lightSource);
	lightRoot->addChild(position);
	return lightRoot ;
}

int main()
{
	osgViewer::Viewer* viewer = new osgViewer::Viewer();

	osg::Group* root = new osg::Group();

	//��ȡģ��
	osg::Node* node = new osg::Node() ;
	node = osgDB::readNodeFile("F:/3rdpart/OSG/OpenSceneGraph/data/dumptruck.osgt");

	//�򳡾�����ӹ�Դ
	root->addChild(createLight1(node));
	//root->addChild(node);

	//�Ż���������
	osgUtil::Optimizer optimizer ;
	optimizer.optimize(root) ;

	viewer->setSceneData(root);

	viewer->realize();

	viewer->run();

	return 0 ;
}