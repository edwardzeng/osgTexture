/**********************************************************
*Write by FlySky
*zzuxp@163.com  http://ww.OsgChina.org   
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
#include <osg/TextureRectangle>
#include <osg/TexMat>

#include <osgDB/ReadFile>
#include <osgDB/WriteFile>

#include <osgUtil/Optimizer>

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

//������ά����״̬����
osg::StateSet* createTexture2DState(osg::Image* image)
{
	//����״̬������
	osg::StateSet* stateset = new osg::StateSet();

	//������ά�������
	osg::TextureRectangle* texture = new osg::TextureRectangle();
	texture->setDataVariance(osg::Object::DYNAMIC);
	//������ͼ
	texture->setImage(image);

	//����������󣬲�����Ϊ���ݾ�������(TextureRectangle)�Ĵ�С�Զ�����
	//�Ӷ�����Ӧ��һ����������һ���������겻��0-1��
	osg::TexMat* texmat = new osg::TexMat;
	texmat->setScaleByTextureRectangleSize(true);

	//���������������
	stateset->setTextureAttributeAndModes(0,texmat,osg::StateAttribute::ON);
	stateset->setTextureAttributeAndModes(0,texture,osg::StateAttribute::ON);

	//�رչ���
	stateset->setMode(GL_LIGHTING,osg::StateAttribute::OFF);

	return stateset ;
}

int main()
{
	osgViewer::Viewer* viewer = new osgViewer::Viewer();

	osg::Group* root = new osg::Group();

	//��ȡ��ͼ�ļ�
	osg::Image* image = new osg::Image();
	image = osgDB::readImageFile("E:/SourceCode/OSG/OSGData/Robot/road.jpg");

	osg::Node* node = new osg::Node();
	node = createNode();

	//����״̬������
	osg::StateSet* stateset = new osg::StateSet();
	stateset = createTexture2DState(image);

	//ʹ�ö�ά����
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