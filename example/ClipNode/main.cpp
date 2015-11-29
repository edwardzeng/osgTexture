/**********************************************************
*Write by FlySky
*zzuxp@163.com  http://www.OsgChina.org   
**********************************************************/

#include <osgViewer/Viewer>

#include <osg/Node>
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/Group>
#include <osg/ClipNode>
#include <osg/PolygonMode>
#include <osg/MatrixTransform>
#include <osg/PositionAttitudeTransform>
#include <osg/AnimationPath>

#include <osgDB/ReadFile>
#include <osgDB/WriteFile>

#include <osgUtil/Optimizer>

#include <iostream>

osg::Node* createClipNode(osg::Node* subgraph)
{
	osg::Group* root = new osg::Group();

	osg::StateSet* stateset = new osg::StateSet();

	//��������λ���ģʽ������ͷ��涼����
	osg::PolygonMode* polymode = new osg::PolygonMode();
	polymode->setMode(osg::PolygonMode::FRONT_AND_BACK,osg::PolygonMode::LINE);

	//���ö�������λ���ģʽ����ָ��״̬�̳�����ΪOVERRIDE
	stateset->setAttributeAndModes(polymode,osg::StateAttribute::OVERRIDE|osg::StateAttribute::ON);

	//��������λ��ƽڵ�
	osg::Group* wireframe_subgraph = new osg::Group;
	//������Ⱦ״̬
	wireframe_subgraph->setStateSet(stateset);
	wireframe_subgraph->addChild(subgraph);

	root->addChild(wireframe_subgraph);

	osg::MatrixTransform* transform= new osg::MatrixTransform;
	//���»ص���ʵ�ֶ�̬�ü�
	osg::NodeCallback* nc = new osg::AnimationPathCallback(subgraph->getBound().center(),osg::Vec3(0.0f,0.0f,1.0f),osg::inDegrees(45.0f));
	transform->setUpdateCallback(nc);

	//�����ü��ڵ�
	osg::ClipNode* clipnode = new osg::ClipNode;
	osg::BoundingSphere bs = subgraph->getBound();
	bs.radius()*= 0.4f;

	//���òü��ڵ�İ�Χ��
	osg::BoundingBox bb;
	bb.expandBy(bs);

	//����ǰ��ָ���İ�Χ�д��������ü�ƽ��
	clipnode->createClipBox(bb);
	//���ü�ѡ
	clipnode->setCullingActive(false);

	transform->addChild(clipnode);
	root->addChild(transform);

	//����δ���ü��Ľڵ�
	osg::Group* clippedNode = new osg::Group;
	clippedNode->setStateSet(clipnode->getStateSet());
	clippedNode->addChild(subgraph);

	root->addChild(clippedNode);

	return root;
}

int main()
{
	osgViewer::Viewer* viewer = new osgViewer::Viewer();

	osg::Node* root = new osg::Node() ;

	//����ģ��
	osg::Node* node = new osg::Node() ;
	node =osgDB::readNodeFile("E:/3rdpart/OSG/OpenSceneGraph-3.0.1-VS10.0.30319-x64/data/cessna.osg");

	root = createClipNode(node);

	//�Ż���������
	osgUtil::Optimizer optimzer;
	optimzer.optimize(root);

	viewer->setSceneData(root);

	viewer->realize();

	viewer->run();

	return 0 ;
}
