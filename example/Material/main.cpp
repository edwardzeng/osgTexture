/**********************************************************
*Write by FlySky
*zzuxp@163.com  http://www.OsgChina.org   
**********************************************************/

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

#include <iostream>

//����һ���ı��νڵ�
osg::Node* createNode()
{
	osg::Geode* geode = new osg::Geode();

	osg::Geometry* geom = new osg::Geometry();

	//���ö���
	osg::Vec3Array* vc= new osg::Vec3Array();
	vc->push_back(osg::Vec3(0.0f,0.0f,0.0f));
	vc->push_back(osg::Vec3(1.0f,0.0f,0.0f));
	vc->push_back(osg::Vec3(1.0f,0.0f,1.0f));
	vc->push_back(osg::Vec3(0.0f,0.0f,1.0f));

	geom->setVertexArray(vc);

	//������������
	osg::Vec2Array* vt= new osg::Vec2Array();
	vt->push_back(osg::Vec2(0.0f,0.0f));
	vt->push_back(osg::Vec2(1.0f,0.0f));
	vt->push_back(osg::Vec2(1.0f,1.0f));
	vt->push_back(osg::Vec2(0.0f,1.0f));

	geom->setTexCoordArray(0,vt);

	//���÷���
	osg::Vec3Array* nc = new osg::Vec3Array();
	nc->push_back(osg::Vec3(0.0f,-1.0f,0.0f));

	geom->setNormalArray(nc);
	geom->setNormalBinding(osg::Geometry::BIND_OVERALL);

	//���ͼԪ
	geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS,0,4));

	//����
	geode->addDrawable(geom);

	return (osg::Node*)(geode);
}

int main()
{
	osgViewer::Viewer* viewer = new osgViewer::Viewer();

	osg::Group* root = new osg::Group();

	osg::Node* node = new osg::Node() ;
	node =createNode();

	//�õ�״̬����
	osg::StateSet* stateset = new osg::StateSet();
	stateset= node->getOrCreateStateSet() ;

	//�������ʶ���
	osg::Material* mat = new osg::Material();
	//��������ɢ����ɫ
	mat->setDiffuse(osg::Material::FRONT,osg::Vec4(1.0f,0.0f,0.0f,1.0f));
	//�������澵����ɫ
	mat->setSpecular(osg::Material::FRONT,osg::Vec4(1.0f,0.0f,0.0f,1.0f));
	//��������ָ��
	mat->setShininess(osg::Material::FRONT,90.0f);
	stateset->setAttribute(mat);

	//���ñ����޳�
	osg::CullFace* cullface = new osg::CullFace(osg::CullFace::BACK) ;
	stateset->setAttribute(cullface);
	stateset->setMode(GL_CULL_FACE,osg::StateAttribute::ON);

	root->addChild(node);

	//�Ż���������
	osgUtil::Optimizer optimizer ;
	optimizer.optimize(root) ;

	viewer->setSceneData(root);

	viewer->realize();

	viewer->run();

	return 0 ;
}