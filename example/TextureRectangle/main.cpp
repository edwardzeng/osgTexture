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

	vc->push_back(osg::Vec3(0.0f,0.0f,1.0f));
	vc->push_back(osg::Vec3(1.0f,0.0f,1.0f));
	vc->push_back(osg::Vec3(1.0f,0.0f,2.0f));
	vc->push_back(osg::Vec3(0.0f,0.0f,2.0f));

	vc->push_back(osg::Vec3(0.0f,0.0f,2.0f));
	vc->push_back(osg::Vec3(1.0f,0.0f,2.0f));
	vc->push_back(osg::Vec3(1.0f,0.0f,3.0f));
	vc->push_back(osg::Vec3(0.0f,0.0f,3.0f));
	geom->setVertexArray(vc);

	//������������
	osg::Vec2Array* vt= new osg::Vec2Array();
	vt->push_back(osg::Vec2(0.0f,0.0f));
	vt->push_back(osg::Vec2(1.0f,0.0f));
	vt->push_back(osg::Vec2(1.0f,1.0f));
	vt->push_back(osg::Vec2(0.0f,1.0f));

	vt->push_back(osg::Vec2(0.0f,0.0f));
	vt->push_back(osg::Vec2(1.0f,0.0f));
	vt->push_back(osg::Vec2(1.0f,1.0f));
	vt->push_back(osg::Vec2(0.0f,1.0f));

	vt->push_back(osg::Vec2(0.0f,0.0f));
	vt->push_back(osg::Vec2(1.0f,0.0f));
	vt->push_back(osg::Vec2(1.0f,1.0f));
	vt->push_back(osg::Vec2(0.0f,1.0f));

	geom->setTexCoordArray(0,vt);

	//���÷���
	osg::Vec3Array* nc = new osg::Vec3Array();
	nc->push_back(osg::Vec3(0.0f,-1.0f,0.0f));//ָ����Ļ

	geom->setNormalArray(nc);
	geom->setNormalBinding(osg::Geometry::BIND_OVERALL);

	//���ͼԪ
	int faceNum=0;
	for (int i=0;i<3;++i)
	{
		geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS,faceNum,4));
		faceNum +=4;
	}
	
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

	osg::Texture2D* texture = new osg::Texture2D;
	texture->setDataVariance(osg::Object::STATIC);
	//������ͼ
	texture->setImage(image);

	//���������������
	stateset->setTextureAttributeAndModes(0,texture,osg::StateAttribute::ON);

	//�رչ���
	stateset->setMode(GL_LIGHTING,osg::StateAttribute::OFF);
	stateset->setTextureAttribute(0,texture,osg::StateAttribute::OVERRIDE);
	stateset->setTextureMode(0,GL_TEXTURE_2D,osg::StateAttribute::ON|osg::StateAttribute::OVERRIDE);

	//������������������
	//stateset->setTextureMode(0,GL_TEXTURE_GEN_S,osg::StateAttribute::ON|osg::StateAttribute::OVERRIDE);
	//stateset->setTextureMode(0,GL_TEXTURE_GEN_T,osg::StateAttribute::ON|osg::StateAttribute::OVERRIDE);

	return stateset ;
}

int main()
{
	osgViewer::Viewer* viewer = new osgViewer::Viewer();

	osg::Group* root = new osg::Group();

	//��ȡ��ͼ�ļ�
	osg::Image* image = new osg::Image();
	image = osgDB::readImageFile("F:/3rdpart/OSG/OpenSceneGraph/data/Images/primitives.jpg");

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
	viewer->setUpViewInWindow(300,100,800,600);
	viewer->setSceneData(root);
	viewer->realize();
	viewer->run();
	return 0 ;
}