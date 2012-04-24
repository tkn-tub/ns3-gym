.. include:: replace.txt

..
	========================================================================================
	Translated for portuguese by the students of the inter-institutional doctorate program of IME-USP/UTFPR-CM.
	
	Traduzido para o português pelos alunos do programa de doutorado inter institucional do Instituto de Matemática e Estatística da Universidade de São Paulo --- IME-USP em parceria com a Universidade Tecnológica Federal do Paraná - Campus Campo Mourão --- UTFPR-CM:
	
	* Frank Helbert (frank@ime.usp.br);
	* Luiz Arthur Feitosa dos Santos (luizsan@ime.usp.br);
	* Rodrigo Campiolo (campiolo@ime.usp.br).
	========================================================================================


.. Conceptual Overview

Visão Conceitual
----------------

..
	The first thing we need to do before actually starting to look at or write
	|ns3| code is to explain a few core concepts and abstractions in the
	system.  Much of this may appear transparently obvious to some, but we
	recommend taking the time to read through this section just to ensure you
	are starting on a firm foundation.

Antes de escrever códigos no |ns3| é extremamente importante entender um pouco dos conceitos e abstrações do sistema. Muitos conceitos poderão parecer óbvios, mas a recomendação geral é que esta seção seja lida por completo para assegurar que o leitor inicie com uma base sólida. 

.. Key Abstractions

Principais Abstrações
*********************

..
	In this section, we'll review some terms that are commonly used in
	networking, but have a specific meaning in |ns3|.

Nesta seção, são revistos alguns termos que são comumente usados por profissionais de redes de computadores, mas que tem um significado específico no |ns3|.

Nó (`Node`)
+++++++++++

.. 
	In Internet jargon, a computing device that connects to a network is called
	a *host* or sometimes an *end system*.  Because |ns3| is a 
	*network* simulator, not specifically an *Internet* simulator, we 
	intentionally do not use the term host since it is closely associated with
	the Internet and its protocols.  Instead, we use a more generic term also
	used by other simulators that originates in Graph Theory --- the *node*.

No jargão da Internet, um dispositivo computacional que conecta-se a uma rede é chamado de *host* ou em alguns casos de *terminal*. Devido ao fato do |ns3| ser um simulador de *rede*, e não um simulador da *Internet*, o termo `host` é intencionalmente não utilizado, pois está intimamente associado com a Internet e seus protocolos. Ao invés disso, é utilizado o termo *node* --- em português, *nó* --- que é um termo mais genérico e também usado por outros simuladores que tem suas origens na Teoria dos Grafos.

..
	In |ns3| the basic computing device abstraction is called the 
	node.  This abstraction is represented in C++ by the class ``Node``.  The 
	``Node`` class provides methods for managing the representations of 
	computing devices in simulations.

A abstração de um dispositivo computacional básico é chamado então de nó. Essa abstração é representada em C++ pela classe ``Node``. Esta classe fornece métodos para gerenciar as representações de dispositivos computacionais nas simulações.

..
	You should think of a ``Node`` as a computer to which you will add 
	functionality.  One adds things like applications, protocol stacks and
	peripheral cards with their associated drivers to enable the computer to do
	useful work.  We use the same basic model in |ns3|.

O nó deve ser pensado como um computador no qual se adicionam funcionalidades, tal como aplicativos, pilhas de protocolos e periféricos com seus `drivers` associados que permitem ao computador executar tarefas úteis. No |ns3| é utilizado este mesmo conceito básico.

.. Application

Aplicações (`Application`)
++++++++++++++++++++++++++

..
	Typically, computer software is divided into two broad classes.  *System
	Software* organizes various computer resources such as memory, processor
	cycles, disk, network, etc., according to some computing model.  System
	software usually does not use those resources to complete tasks that directly
	benefit a user.  A user would typically run an *application* that acquires
	and uses the resources controlled by the system software to accomplish some
	goal.  

Normalmente, programas de computador são divididos em duas classes. *Programas de sistema* organizam recursos do computador, tais como: memória, processador, disco, rede, etc., de acordo com algum modelo computacional. Tais programas normalmente não são utilizados diretamente pelos usuários. Na maioria das vezes, os usuários fazem uso de *aplicações*, que usam os recursos controlados pelos programas de sistema para atingir seus objetivos.

..
	Often, the line of separation between system and application software is made
	at the privilege level change that happens in operating system traps.
	In |ns3| there is no real concept of operating system and especially
	no concept of privilege levels or system calls.  We do, however, have the
	idea of an application.  Just as software applications run on computers to
	perform tasks in the "real world," |ns3| applications run on
	|ns3| ``Nodes`` to drive simulations in the simulated world.

Geralmente, a separação entre programas de sistema e aplicações de usuários é feita pela mudança no nível de privilégios que acontece na troca de contexto feita pelo sistema operacional. No |ns3|, não existe um conceito de sistema operacional real, não há o conceito de níveis de privilégios nem chamadas de sistema. Há apenas aplicações que são executadas nos nós para uma determinada simulação.

..
	In |ns3| the basic abstraction for a user program that generates some
	activity to be simulated is the application.  This abstraction is represented 
	in C++ by the class ``Application``.  The ``Application`` class provides 
	methods for managing the representations of our version of user-level 
	applications in simulations.  Developers are expected to specialize the
	``Application`` class in the object-oriented programming sense to create new
	applications.  In this tutorial, we will use specializations of class 
	``Application`` called ``UdpEchoClientApplication`` and 
	``UdpEchoServerApplication``.  As you might expect, these applications 
	compose a client/server application set used to generate and echo simulated 
	network packets 

No |ns3|, a abstração básica para um programa de usuário que gera alguma atividade a ser simulada é a aplicação. Esta abstração é representada em C++ pela classe ``Application``, que fornece métodos para gerenciar a representação de suas versões de aplicações a serem simuladas. Os desenvolvedores devem especializar a classe ``Application`` para criar novas aplicações. Neste tutorial serão utilizadas duas especializações da classe ``Application``, chamadas  ``UdpEchoClientApplication`` e ``UdpEchoServerApplication``. Estas aplicações compõem um modelo cliente/servidor usado para gerar pacotes simulados de eco na rede.

.. Channel

Canal de Comunicação (`Channel`)
++++++++++++++++++++++++++++++++

..
	In the real world, one can connect a computer to a network.  Often the media
	over which data flows in these networks are called *channels*.  When
	you connect your Ethernet cable to the plug in the wall, you are connecting 
	your computer to an Ethernet communication channel.  In the simulated world
	of |ns3|, one connects a ``Node`` to an object representing a
	communication channel.  Here the basic communication subnetwork abstraction 
	is called the channel and is represented in C++ by the class ``Channel``.  

No mundo real, computadores estão conectados em uma rede. Normalmente, o meio sobre o qual os dados trafegam é chamada de canal (*channel*). Quando um cabo Ethernet é ligado ao conector na parede, na verdade está se conectando a um canal de comunicação Ethernet. No mundo simulado do |ns3|, um nó é conectado a um objeto que representa um canal de comunicação. A abstração de canal de comunicação é representada em C++ pela classe ``Channel``.

..
	The ``Channel`` class provides methods for managing communication 
	subnetwork objects and connecting nodes to them.  ``Channels`` may also be
	specialized by developers in the object oriented programming sense.  A 
	``Channel`` specialization may model something as simple as a wire.  The 
	specialized  ``Channel`` can also model things as complicated as a large 
	Ethernet switch, or three-dimensional space full of obstructions in the case 
	of wireless networks.

A classe ``Channel`` fornece métodos para gerenciar objetos de comunicação de sub-redes e nós conectados a eles. Os ``Channels`` também podem ser especializados por desenvolvedores (no sentido de programação orientada a objetos). Uma especialização de ``Channel`` pode ser algo como um simples fio. Pode também ser algo mais complexo, como um comutador Ethernet ou ainda ser uma rede sem fio (`wireless`) em um espaço tridimensional com obstáculos.

..
	We will use specialized versions of the ``Channel`` called
	``CsmaChannel``, ``PointToPointChannel`` and ``WifiChannel`` in this
	tutorial.  The ``CsmaChannel``, for example, models a version of a 
	communication subnetwork that implements a *carrier sense multiple 
	access* communication medium.  This gives us Ethernet-like functionality.  

Neste tutorial, são utilizadas versões especializadas de ``Channel`` chamadas ``CsmaChannel``, ``PointToPointChannel`` e ``WifiChannel``. O ``CsmaChannel``, por exemplo, é uma versão do modelo de rede que implementa controle de acesso ao meio CSMA (*Carrier Sense Multiple Access*). Ele fornece uma funcionalidade similar a uma rede Ethernet.

.. Net Device

Dispositivos de Rede (`Net Device`)
+++++++++++++++++++++++++++++++++++

..
	It used to be the case that if you wanted to connect a computers to a network,
	you had to buy a specific kind of network cable and a hardware device called
	(in PC terminology) a *peripheral card* that needed to be installed in
	your computer.  If the peripheral card implemented some networking function,
	they were called Network Interface Cards, or *NICs*.  Today most 
	computers come with the network interface hardware built in and users don't 
	see these building blocks.

No passado, para conectar computadores em uma rede, era necessário comprar o cabo específico e um dispositivo chamado (na terminologia dos PCs) de *periférico*, que precisava ser instalado no computador. Se a placa implementava funções de rede, era chamada de interface de rede (`Network Interface Card` - NIC). Atualmente, a maioria dos computadores vêm com a placa de rede integrada à placa mãe (`on-board`) e os usuários não vêem o computador como uma junção de partes.

..
	A NIC will not work without a software driver to control the hardware.  In 
	Unix (or Linux), a piece of peripheral hardware is classified as a 
	*device*.  Devices are controlled using *device drivers*, and network
	devices (NICs) are controlled using *network device drivers*
	collectively known as *net devices*.  In Unix and Linux you refer
	to these net devices by names such as *eth0*.

Uma placa de rede não funciona sem o `driver` que a controle. No Unix (ou Linux), um periférico (como a placa de rede) é classificado como um dispositivo (*device*). Dispositivos são controlados usando drivers de dispositivo (*device drivers*) e as placas de rede são controladas através de drivers de dispositivo de rede (*network device drivers*), também chamadas de dispositivos de rede (*net devices*). No Unix e Linux estes dispositivos de rede levam nomes como *eth0*.

..
	In |ns3| the *net device* abstraction covers both the software 
	driver and the simulated hardware.  A net device is "installed" in a 
	``Node`` in order to enable the ``Node`` to communicate with other 
	``Nodes`` in the simulation via ``Channels``.  Just as in a real
	computer, a ``Node`` may be connected to more than one ``Channel`` via
	multiple ``NetDevices``.

No |ns3| a abstração do *dispositivo de rede* cobre tanto o hardware quanto o software (`drive`). Um dispositivo de rede é "instalado" em um nó para permitir que este se comunique com outros na simulação, usando os canais de comunicação (`channels`). Assim como em um computador real, um nó pode ser conectado a mais que um canal via múltiplos dispositivos de rede.

..
	The net device abstraction is represented in C++ by the class ``NetDevice``.
	The ``NetDevice`` class provides methods for managing connections to 
	``Node`` and ``Channel`` objects; and may be specialized by developers
	in the object-oriented programming sense.  We will use the several specialized
	versions of the ``NetDevice`` called ``CsmaNetDevice``,
	``PointToPointNetDevice``, and ``WifiNetDevice`` in this tutorial.
	Just as an Ethernet NIC is designed to work with an Ethernet network, the
	``CsmaNetDevice`` is designed to work with a ``CsmaChannel``; the
	``PointToPointNetDevice`` is designed to work with a 
	``PointToPointChannel`` and a ``WifiNetNevice`` is designed to work with
	a ``WifiChannel``.

A abstração do dispositivo de rede é representado em C++ pela classe ``NetDevice``, que fornece métodos para gerenciar conexões para objetos ``Node`` e ``Channel``. Os dispositivos, assim como os canais e as aplicações, também podem ser especializados. Várias versões do ``NetDevice`` são utilizadas neste tutorial, tais como: ``CsmaNetDevice``, ``PointToPointNetDevice`` e ``WifiNetDevice``. Assim como uma placa de rede Ethernet é projetada para trabalhar em uma rede Ethernet, um ``CsmaNetDevice`` é projetado para trabalhar com um ``CsmaChannel``. O ``PointToPointNetDevice`` deve trabalhar com um ``PointToPointChannel`` e o ``WifiNetNevice`` com um ``WifiChannel``.

.. Topology Helpers

Assistentes de Topologia (`Topology Helpers`)
+++++++++++++++++++++++++++++++++++++++++++++

..
	In a real network, you will find host computers with added (or built-in)
	NICs.  In |ns3| we would say that you will find ``Nodes`` with 
	attached ``NetDevices``.  In a large simulated network you will need to 
	arrange many connections between ``Nodes``, ``NetDevices`` and 
	``Channels``.

Em redes reais, os computadores possuem placas de rede, sejam elas integradas ou não. No |ns3|, teremos nós com dispositivos de rede. Em grandes simulações, será necessário arranjar muitas conexões entre nós, dispositivos e canais de comunicação.

..
	Since connecting ``NetDevices`` to ``Nodes``, ``NetDevices``
	to ``Channels``, assigning IP addresses,  etc., are such common tasks
	in |ns3|, we provide what we call *topology helpers* to make 
	this as easy as possible.  For example, it may take many distinct 
	|ns3| core operations to create a NetDevice, add a MAC address, 
	install that net device on a ``Node``, configure the node's protocol stack,
	and then connect the ``NetDevice`` to a ``Channel``.  Even more
	operations would be required to connect multiple devices onto multipoint 
	channels and then to connect individual networks together into internetworks.
	We provide topology helper objects that combine those many distinct operations
	into an easy to use model for your convenience.

Visto que conectar dispositivos a nós e a canais, atribuir endereços IP, etc., são todas tarefas rotineiras no |ns3|, são fornecidos os Assistentes de Topologia (*topology helpers*). Por exemplo, podem ser necessárias muitas operações distintas para criar um dispositivo, atribuir um endereço MAC a ele, instalar o dispositivo em um nó, configurar a pilha de protocolos no nó em questão e por fim, conectar o dispositivo ao canal. Ainda mais operações podem ser necessárias para conectar múltiplos dispositivos em canais multiponto e então fazer a interconexão das várias redes. Para facilitar o trabalho, são disponibilizados objetos que são Assistentes de Topologia, que combinam estas operações distintas em um modelo fácil e conveniente.

.. A First ns-3 Script

O primeiro código no ns-3
*************************
..
	If you downloaded the system as was suggested above, you will have a release
	of |ns3| in a directory called ``repos`` under your home 
	directory.  Change into that release directory, and you should find a 
	directory structure something like the following:

Se o sistema foi baixado como sugerido, uma versão do |ns3| estará em um diretório chamado ``repos`` dentro do diretório `home`. Entrando no diretório dessa versão, deverá haver uma estrutura parecida com a seguinte:

::

  AUTHORS       examples       scratch        utils      waf.bat*
  bindings      LICENSE        src            utils.py   waf-tools
  build         ns3            test.py*       utils.pyc  wscript
  CHANGES.html  README         testpy-output  VERSION    wutils.py
  doc           RELEASE_NOTES  testpy.supp    waf*       wutils.pyc


..
	Change into the ``examples/tutorial`` directory.  You should see a file named 
	``first.cc`` located there.  This is a script that will create a simple
	point-to-point link between two nodes and echo a single packet between the
	nodes.  Let's take a look at that script line by line, so go ahead and open
	``first.cc`` in your favorite editor.

Entrando no diretório ``examples/tutorial``, vai haver um arquivo chamado ``first.cc``. Este é um código que criará uma conexão ponto-a-ponto entre dois nós e enviará um pacote de eco entre eles. O arquivo será analisado linha a linha, para isto, o leitor pode abri-lo em seu editor de textos favorito.

.. 
	Boilerplate

Padronização
++++++++++++

..
	The first line in the file is an emacs mode line.  This tells emacs about the
	formatting conventions (coding style) we use in our source code.  

A primeira linha do arquivo é uma linha de modo emacs, que informa sobre a convenção de formatação (estilo de codificação) que será usada no código fonte.

::

  /* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

..
	This is always a somewhat controversial subject, so we might as well get it
	out of the way immediately.  The |ns3| project, like most large 
	projects, has adopted a coding style to which all contributed code must 
	adhere.  If you want to contribute your code to the project, you will 
	eventually have to conform to the |ns3| coding standard as described 
	in the file ``doc/codingstd.txt`` or shown on the project web page
	`here
	<http://www.nsnam.org/developers/contributing-code/coding-style/>`_.

Este é sempre um assunto um tanto quanto controverso. O projeto |ns3|, tal como a maioria dos projetos de grande porte, adotou um estilo de codificação, para o qual todo o código deve conformar. Se o leitor quiser contribuir com o projeto, deverá estar em conformidade com a codificação que está descrita no arquivo ``doc/codingstd.txt`` ou no `sítio <http://www.nsnam.org/developers/contributing-code/coding-style/>`_.

..
	We recommend that you, well, just get used to the look and feel of |ns3|
	code and adopt this standard whenever you are working with our code.  All of 
	the development team and contributors have done so with various amounts of 
	grumbling.  The emacs mode line above makes it easier to get the formatting 
	correct if you use the emacs editor.

A equipe do |ns3| recomenda aos novos usuários que adotem o padrão quando estiverem tralhando com o código. Tanto eles, quanto todos os que contribuem, tiveram que fazer isto em algum momento. Para aqueles que utilizam o editor Emacs, a linha de modo torna mais facil seguir o padrão corretamente.

..
	The |ns3| simulator is licensed using the GNU General Public 
	License.  You will see the appropriate GNU legalese at the head of every file 
	in the |ns3| distribution.  Often you will see a copyright notice for
	one of the institutions involved in the |ns3| project above the GPL
	text and an author listed below.

O |ns3| é licenciado usando a `GNU General Public License` - GPL. No cabeçalho de todo aquivo da distribuição há as questões legais associadas à licença. Frequentemente, haverá também informações sobre os direitos de cópia de uma das instituições envolvidas no projeto e o autor do arquivo.

::

  /*
   * This program is free software; you can redistribute it and/or modify
   * it under the terms of the GNU General Public License version 2 as
   * published by the Free Software Foundation;
   *
   * This program is distributed in the hope that it will be useful,
   * but WITHOUT ANY WARRANTY; without even the implied warranty of
   * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   * GNU General Public License for more details.
   *
   * You should have received a copy of the GNU General Public License
   * along with this program; if not, write to the Free Software
   * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
   */

.. Module Includes

Inclusão de Módulos
+++++++++++++++++++

..
	The code proper starts with a number of include statements.  

O código realmente começa pelo carregamento de módulos através da inclusão dos arquivos:

::

  #include "ns3/core-module.h"
  #include "ns3/network-module.h"
  #include "ns3/internet-module.h"
  #include "ns3/point-to-point-module.h"
  #include "ns3/applications-module.h"

..
	To help our high-level script users deal with the large number of include 
	files present in the system, we group includes according to relatively large 
	modules.  We provide a single include file that will recursively load all of 
	the include files used in each module.  Rather than having to look up exactly
	what header you need, and possibly have to get a number of dependencies right,
	we give you the ability to load a group of files at a large granularity.  This
	is not the most efficient approach but it certainly makes writing scripts much
	easier.

Os arquivos a serem incluídos são agrupados em módulos relativamente grandes, de forma a ajudar os usuários. Também é possível fazer referência a um único arquivo que irá recursivamente carregar todas as bibliotecas de cada módulo. Ao invés de procurar pelo arquivo exato, e provavelmente, ter que resolver dependências, é possível carregar um grupo de arquivos de uma vez. Esta com certeza não é a abordagem mais eficiente, mas permite escrever códigos de forma bem mais fácil.


..
	Each of the |ns3| include files is placed in a directory called 
	``ns3`` (under the build directory) during the build process to help avoid
	include file name collisions.  The ``ns3/core-module.h`` file corresponds 
	to the ns-3 module you will find in the directory ``src/core`` in your 
	downloaded release distribution.  If you list this directory you will find a
	large number of header files.  When you do a build, Waf will place public 
	header files in an ``ns3`` directory under the appropriate 
	``build/debug`` or ``build/optimized`` directory depending on your 
	configuration.  Waf will also automatically generate a module include file to
	load all of the public header files.

Durante a construção, cada um dos arquivos incluídos é copiado para o diretório chamado ``ns3`` (dentro do diretório ``build``), o que ajuda a evitar conflito entre os nomes dos arquivos de bibliotecas. O arquivo ``ns3/core-module.h``, por exemplo, corresponde ao módulo que está no diretório ``src/core``. Há um grande número de arquivos neste diretório. No momento em que o Waf está construindo o projeto, copia os arquivos para o diretório ``ns3``, no subdiretório apropriado --- ``build/debug`` ou ``build/optimized`` --- dependendo da configuração utilizada.

..
	Since you are, of course, following this tutorial religiously, you will 
	already have done a

Considerando que o leitor esteja seguindo este tutorial, já terá feito:

::

  ./waf -d debug --enable-examples --enable-tests configure

..
	in order to configure the project to perform debug builds that include 
	examples and tests.  You will also have done a

Também já terá feito

::

  ./waf

..
	to build the project.  So now if you look in the directory 
	``../../build/debug/ns3`` you will find the four module include files shown 
	above.  You can take a look at the contents of these files and find that they
	do include all of the public include files in their respective modules.

para construir o projeto. Então, no diretório ``../../build/debug/ns3`` deverá haver os quatro módulos incluídos anteriormente. Olhando para o conteúdo destes arquivos, é possível observar que eles incluem todos os arquivos públicos dos seus respectivos módulos.


Ns3 `Namespace`
+++++++++++++++

..
	The next line in the ``first.cc`` script is a namespace declaration.

A próxima linha no código ``first.cc`` é a declaração do `namespace`.

::

  using namespace ns3;

..
	The |ns3| project is implemented in a C++ namespace called 
	``ns3``.  This groups all |ns3|-related declarations in a scope
	outside the global namespace, which we hope will help with integration with 
	other code.  The C++ ``using`` statement introduces the |ns3|
	namespace into the current (global) declarative region.  This is a fancy way
	of saying that after this declaration, you will not have to type ``ns3::``
	scope resolution operator before all of the |ns3| code in order to use
	it.  If you are unfamiliar with namespaces, please consult almost any C++ 
	tutorial and compare the ``ns3`` namespace and usage here with instances of
	the ``std`` namespace and the ``using namespace std;`` statements you 
	will often find in discussions of ``cout`` and streams.

O projeto |ns3| é implementado em um `namespace` chamado `ns3`. Isto agrupa todas as declarações relacionadas ao projeto em um escopo fora do global, que ajuda na integração com outros códigos. A declaração ``using`` do C++ insere o `namespace` |ns3| no escopo global, evitando que se tenha que ficar digitando ``ns3::`` antes dos códigos |ns3|. Se o leitor não esta familiarizado com `namesapaces`, consulte algum tutorial de C++ e compare o `namespace` ``ns3`` e o `namespace` ``std``, usado com frequência em C++, principalmente com ``cout`` e ``streams``.

.. Logging

Registro (`Logging`)
++++++++++++++++++++
..
	The next line of the script is the following,

A próxima linha do código é o seguinte,

::

  NS_LOG_COMPONENT_DEFINE ("FirstScriptExample");

..
	We will use this statement as a convenient place to talk about our Doxygen
	documentation system.  If you look at the project web site, 
	`ns-3 project
	<http://www.nsnam.org>`_, you will find a link to "Documentation" in the navigation bar.  If you select this link, you will be
	taken to our documentation page. There 
	is a link to "Latest Release" that will take you to the documentation
	for the latest stable release of |ns3|.
	If you select the "API Documentation" link, you will be
	taken to the |ns3| API documentation page.

Nós iremos utilizar esta declaração em um lugar conveniente para conversar com o sistema de documentação Doxygen. Se você procurar no `web site` do `projeto ns-3 <http://www.nsnam.org>`_, você encontrará um `link` para a documentação (`Documentation`) na barra de navegação. Se selecionarmos este `link`, veremos a página de documentação. Lá tem um `link` para as últimas `releases` (`Latest Release`) que irão apresentar a documentação da última `release` do |ns3|. Se você também pode selecionar o `link` para a documentação das APIs (`API Documentation`).

.. 
	Along the left side, you will find a graphical representation of the structure
	of the documentation.  A good place to start is the ``NS-3 Modules``
	"book" in the |ns3| navigation tree.  If you expand ``Modules`` 
	you will see a list of |ns3| module documentation.  The concept of 
	module here ties directly into the module include files discussed above.  The |ns3| logging subsystem is discussed in the ``C++ Constructs Used by All Modules`` 
	section, so go ahead and expand that documentation node.  Now, expand the 
	``Debugging`` book and then select the ``Logging`` page.

Do lado esquerdo, você achará uma representação gráfica da estrutura da documentação. Um bom lugar para começar é com o livro de módulos do NS-3 (``NS-3 Modules``) na árvore de navegação, você pode expandir para ver a lista de documentação de módulos do |ns3|. O conceito de módulo aqui está diretamente ligado com a inclusão de bibliotecas apresentadas anteriormente. O sistema de registro (`logging`) é discutido na seção ``C++ Constructs Used by All Modules``, vá em frente e expanda a documentação. Agora expanda o livro da depuração (``Debugging``) e selecione a página de ``Logging``.

..
	You should now be looking at the Doxygen documentation for the Logging module.
	In the list of ``#define``s at the top of the page you will see the entry
	for ``NS_LOG_COMPONENT_DEFINE``.  Before jumping in, it would probably be 
	good to look for the "Detailed Description" of the logging module to get a 
	feel for the overall operation.  You can either scroll down or select the
	"More..." link under the collaboration diagram to do this.

Agora você deve procurar na documentação Doxygen pelo módulo de ``Logging``. Na lista de ``#define`` bem no topo da página você verá uma entrada para ``NS_LOG_COMPONENT_DEFINE``. Antes de ir para lá, dê uma boa olhada na descrição detalhada (`Detailed Description`) do módulo de `logging`.

..
	Once you have a general idea of what is going on, go ahead and take a look at
	the specific ``NS_LOG_COMPONENT_DEFINE`` documentation.  I won't duplicate
	the documentation here, but to summarize, this line declares a logging 
	component called ``FirstScriptExample`` that allows you to enable and 
	disable console message logging by reference to the name.

Uma vez que você tem uma ideia geral, prossiga e olhe a documentação de ``NS_LOG_COMPONENT_DEFINE``. Não esperamos duplicar a documentação, mas para resumir esta linha declara o componente de `logging` chamado ``FirstScriptExample`` que permite habilitar e desabilitar mensagens de `logging` referenciando pelo nome.

.. Main Function

Função Principal
++++++++++++++++

..
	The next lines of the script you will find are,

As próximas linhas do código são,

::

  int
  main (int argc, char *argv[])
  {

..
	This is just the declaration of the main function of your program (script).
	Just as in any C++ program, you need to define a main function that will be 
	the first function run.  There is nothing at all special here.  Your 
	|ns3| script is just a C++ program.

Esta é a declaração da função principal (``main``) do programa. Assim como em qualquer programa em C++, você precisa definir uma função principal que é a primeira função que será executada no programa. Não há nada de especial e seu código |ns3| é apenas um programa C++.

..
	The next two lines of the script are used to enable two logging components that
	are built into the Echo Client and Echo Server applications:

As próximas duas linhas do código são usadas para habilitar dois componentes de registro que são construídos com as aplicações de `Echo Client` e `Echo Server`:

::

    LogComponentEnable("UdpEchoClientApplication", LOG_LEVEL_INFO);
    LogComponentEnable("UdpEchoServerApplication", LOG_LEVEL_INFO);

..
	If you have read over the Logging component documentation you will have seen
	that there are a number of levels of logging verbosity/detail that you can 
	enable on each component.  These two lines of code enable debug logging at the
	INFO level for echo clients and servers.  This will result in the application
	printing out messages as packets are sent and received during the simulation.

Se você leu a documentação do componente de `logging` você viu que existem vários níveis de detalhamento de `logging` e que você pode habilitá-los para cada componente. Essas duas linhas de código habilitam a depuração de `logging` com o nível ``INFO`` para o cliente e servidor. Isto ira fazer com que as aplicações mostrem as mensagens dos pacotes sendo enviados e recebidos durante a simulação.

..
	Now we will get directly to the business of creating a topology and running 
	a simulation.  We use the topology helper objects to make this job as
	easy as possible.


Agora nós iremos direto ao ponto, criando uma topologia e executando uma simulação. Nós usaremos o Assistente de Topologia  para fazer isto da forma mais simples possível.

.. Topology Helpers

Assistentes de Topologia
++++++++++++++++++++++++

.. NodeContainer

Contêineres de nós (`NodeContainer`)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
.. 
	The next two lines of code in our script will actually create the 
	|ns3| ``Node`` objects that will represent the computers in the 
	simulation.  

As próximas duas linhas de código cria os objetos do tipo ``Node`` que representarão os computadores na simulação.

::

    NodeContainer nodes;
    nodes.Create (2);

..
	Let's find the documentation for the ``NodeContainer`` class before we
	continue.  Another way to get into the documentation for a given class is via 
	the ``Classes`` tab in the Doxygen pages.  If you still have the Doxygen 
	handy, just scroll up to the top of the page and select the ``Classes`` 
	tab.  You should see a new set of tabs appear, one of which is 
	``Class List``.  Under that tab you will see a list of all of the 
	|ns3| classes.  Scroll down, looking for ``ns3::NodeContainer``.
	When you find the class, go ahead and select it to go to the documentation for
	the class.

Antes de continuar vamos pesquisar a documentação da classe ``NodeContainer``. Outra forma de obter a documentação é através aba ``Classes`` na página do Doxygen. No Doxygen, vá até o topo da página e selecione ``Classes``. Então, você verá um novo conjunto de opções aparecendo, uma delas será a sub-aba ``Class List``. Dentro desta opção você verá uma lista com todas as classes do |ns3|. Agora procure por ``ns3::NodeContainer``. Quando você achar a classe, selecione e veja a documentação da classe.

..
	You may recall that one of our key abstractions is the ``Node``.  This
	represents a computer to which we are going to add things like protocol stacks,
	applications and peripheral cards.  The ``NodeContainer`` topology helper
	provides a convenient way to create, manage and access any ``Node`` objects
	that we create in order to run a simulation.  The first line above just 
	declares a NodeContainer which we call ``nodes``.  The second line calls the
	``Create`` method on the ``nodes`` object and asks the container to 
	create two nodes.  As described in the Doxygen, the container calls down into
	the |ns3| system proper to create two ``Node`` objects and stores
	pointers to those objects internally.

Lembre-se que uma de nossas abstrações é o nó. Este representa um computador, ao qual iremos adicionar coisas, como protocolos, aplicações e periféricos. O assistente ``NodeContainer`` fornece uma forma conveniente de criar, gerenciar e acessar qualquer objeto ``Node`` que criamos para executar a simulação. A primeira linha declara um ``NodeContainer`` que chamamos de ``nodes``. A segunda linha chama o método ``Create`` sobre o objeto ``nodes`` e pede para criar dois nós.

..
	The nodes as they stand in the script do nothing.  The next step in 
	constructing a topology is to connect our nodes together into a network.
	The simplest form of network we support is a single point-to-point link 
	between two nodes.  We'll construct one of those links here.

Os nós, como estão no código, não fazem nada. O próximo passo é montar uma topologia para conectá-los em uma rede. Uma forma simples de conectar dois computadores em uma rede é com um enlace ponto-a-ponto.

PointToPointHelper
~~~~~~~~~~~~~~~~~~
..
	We are constructing a point to point link, and, in a pattern which will become
	quite familiar to you, we use a topology helper object to do the low-level
	work required to put the link together.  Recall that two of our key 
	abstractions are the ``NetDevice`` and the ``Channel``.  In the real
	world, these terms correspond roughly to peripheral cards and network cables.  
	Typically these two things are intimately tied together and one cannot expect
	to interchange, for example, Ethernet devices and wireless channels.  Our 
	Topology Helpers follow this intimate coupling and therefore you will use a
	single ``PointToPointHelper`` to configure and connect |ns3|
	``PointToPointNetDevice`` and ``PointToPointChannel`` objects in this 
	script.

Construiremos um enlace ponto-a-ponto e para isto usaremos um assistente para configurar o nível mais baixo da rede. Lembre-se que duas abstrações básicas são ``NetDevice`` e ``Channel``. No mundo real, estes termos correspondem, a grosso modo, à placa de rede e ao cabo. Normalmente, estes dois estão intimamente ligados e não é normal ficar trocando. Por exemplo, não é comum placas Ethernet conectadas em canais sem fio. O assistente de topologia acopla estes dois conceitos em um simples ``PointToPointHelper`` para configurar e conectar objetos ``PointToPointNetDevice`` e ``PointToPointChannel`` em nosso código.

.. 
	The next three lines in the script are,

As próximas três linhas no código são,

::

    PointToPointHelper pointToPoint;
    pointToPoint.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
    pointToPoint.SetChannelAttribute ("Delay", StringValue ("2ms"));

.. 
	The first line,

A primeira linha,

::

    PointToPointHelper pointToPoint;

..
	instantiates a ``PointToPointHelper`` object on the stack.  From a 
	high-level perspective the next line,

instancia o objeto ``PointToPointHelper`` na pilha. De forma mais superficial a próxima linha,

::

    pointToPoint.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));

..
	tells the ``PointToPointHelper`` object to use the value "5Mbps"
	(five megabits per second) as the "DataRate" when it creates a 
	``PointToPointNetDevice`` object.

diz ao objeto ``PointToPointHelper`` para usar o valor de "5Mbps" (cinco `megabits` por segundo) como "DataRate" (taxa de transferência) quando criarmos um objeto ``PointToPointNetDevice``.

..
	From a more detailed perspective, the string "DataRate" corresponds
	to what we call an ``Attribute`` of the ``PointToPointNetDevice``.
	If you look at the Doxygen for class ``ns3::PointToPointNetDevice`` and 
	find the documentation for the ``GetTypeId`` method, you will find a list
	of  ``Attributes`` defined for the device.  Among these is the "DataRate"
	``Attribute``.  Most user-visible |ns3| objects have similar lists of 
	``Attributes``.  We use this mechanism to easily configure simulations without
	recompiling as you will see in a following section.

De uma perspectiva mais detalhada, a palavra "DataRate" corresponde ao que nós chamamos de atributo (``Attribute``) do ``PointToPointNetDevice``. Se você olhar no Doxygen na classe ``ns3::PointToPointNetDevice`` e procurar a documentação para o método ``GetTypeId``, você achará uma lista de atributos definidos por dispositivos. Dentro desses está o atributo "DataRate". A maioria dos objetos do |ns3| tem uma lista similar de atributos. Nós usamos este mecanismo para facilitar a configuração das simulações sem precisar recompilar, veremos isto na seção seguinte.

..
	Similar to the "DataRate" on the ``PointToPointNetDevice`` you will find a
	"Delay" ``Attribute`` associated with the ``PointToPointChannel``.  The 
	final line,

Parecido com o "DataRate" no ``PointToPointNetDevice`` você achará o atributo "Delay" (atraso) associado com o ``PointToPointChannel``. O final da linha,

::

    pointToPoint.SetChannelAttribute ("Delay", StringValue ("2ms"));

..
	tells the ``PointToPointHelper`` to use the value "2ms" (two milliseconds)
	as the value of the transmission delay of every point to point channel it 
	subsequently creates.

diz ao ``PointToPointHelper`` para usar o valor de "2ms" (dois milissegundos) como valor de atraso de transmissão para o canal ponto-a-ponto criado.

NetDeviceContainer
~~~~~~~~~~~~~~~~~~

.. 
	At this point in the script, we have a ``NodeContainer`` that contains
	two nodes.  We have a ``PointToPointHelper`` that is primed and ready to 
	make ``PointToPointNetDevices`` and wire ``PointToPointChannel`` objects
	between them.  Just as we used the ``NodeContainer`` topology helper object
	to create the ``Nodes`` for our simulation, we will ask the 
	``PointToPointHelper`` to do the work involved in creating, configuring and
	installing our devices for us.  We will need to have a list of all of the 
	NetDevice objects that are created, so we use a NetDeviceContainer to hold 
	them just as we used a NodeContainer to hold the nodes we created.  The 
	following two lines of code,

Até agora no código, temos um ``NodeContainer`` que contém dois nós. Também temos ``PointToPointHelper`` que carrega e prepara os objetos ``PointToPointNetDevices`` e ``PointToPointChannel``. Depois, usamos o assistente ``NodeContainer`` para criar os nós para a simulação. Iremos pedir ao ``PointToPointHelper`` para criar, configurar e instalar nossos dispositivos. Iremos necessitar de uma lista de todos os objetos `NetDevice` que são criados, então nós usamos um `NetDeviceContainer` para agrupar os objetos criados, tal como usamos o `NodeContainer` para agrupar os nós que criamos. Nas duas linhas de código seguintes,

::

    NetDeviceContainer devices;
    devices = pointToPoint.Install (nodes);

..
	will finish configuring the devices and channel.  The first line declares the 
	device container mentioned above and the second does the heavy lifting.  The 
	``Install`` method of the ``PointToPointHelper`` takes a 
	``NodeContainer`` as a parameter.  Internally, a ``NetDeviceContainer`` 
	is created.  For each node in the ``NodeContainer`` (there must be exactly 
	two for a point-to-point link) a ``PointToPointNetDevice`` is created and 
	saved in the device container.  A ``PointToPointChannel`` is created and 
	the two ``PointToPointNetDevices`` are attached.  When objects are created
	by the ``PointToPointHelper``, the ``Attributes`` previously set in the 
	helper are used to initialize the corresponding ``Attributes`` in the 
	created objects.

vamos terminar configurando os dispositivos e o canal. A primeira linha declara o contêiner de dispositivos mencionado anteriormente e o segundo faz o trabalho pesado. O método ``Install`` do ``PointToPointHelper`` utiliza um ``NodeContainer`` como parâmetro. Internamente, um ``NetDeviceContainer`` é criado. Para cada nó no ``NodeContainer`` (devem existir dois para um enlace ponto-a-ponto) um ``PointToPointNetDevice`` é criado e salvo no contêiner do dispositivo. Um ``PointToPointChannel`` é criado e dois ``PointToPointNetDevices`` são conectados. Quando os objetos são criados pelo ``PointToPointHelper``, os atributos, passados anteriormente, são configurados pelo assistente (`Helper`).

..
	After executing the ``pointToPoint.Install (nodes)`` call we will have
	two nodes, each with an installed point-to-point net device and a single
	point-to-point channel between them.  Both devices will be configured to 
	transmit data at five megabits per second over the channel which has a two 
	millisecond transmission delay.

Depois de executar a chamada ``pointToPoint.Install (nodes)`` iremos ter dois nós, cada qual instalado na rede ponto-a-ponto e um único canal ponto-a-ponto ligando os dois. Ambos os dispositivos serão configurados para ter uma taxa de transferência de dados de cinco megabits por segundo, que por sua vez tem um atraso de transmissão de dois milissegundos.

InternetStackHelper
~~~~~~~~~~~~~~~~~~~
..
	We now have nodes and devices configured, but we don't have any protocol stacks
	installed on our nodes.  The next two lines of code will take care of that.

Agora temos os nós e dispositivos configurados, mas não temos qualquer pilha de protocolos instalada em nossos nós. As próximas duas linhas de código irão cuidar disso.

::

    InternetStackHelper stack;
    stack.Install (nodes);

..
	The ``InternetStackHelper`` is a topology helper that is to internet stacks
	what the ``PointToPointHelper`` is to point-to-point net devices.  The
	``Install`` method takes a ``NodeContainer`` as a parameter.  When it is
	executed, it will install an Internet Stack (TCP, UDP, IP, etc.) on each of
	the nodes in the node container.

O ``InternetStackHelper`` é um assistente de topologia inter-rede. O método ``Install`` utiliza um ``NodeContainer`` como parâmetro. Quando isto é executado, ele irá instalar a pilha de protocolos da Internet (TCP, UDP, IP, etc) em cada nó do contêiner.

Ipv4AddressHelper
~~~~~~~~~~~~~~~~~

..
	Next we need to associate the devices on our nodes with IP addresses.  We 
	provide a topology helper to manage the allocation of IP addresses.  The only
	user-visible API is to set the base IP address and network mask to use when
	performing the actual address allocation (which is done at a lower level 
	inside the helper).

Agora precisamos associar os dispositivos dos nós a endereços IP. Nós fornecemos um assistente de topologia para gerenciar a alocação de endereços IP's. A única API de usuário visível serve para configurar o endereço IP base e a máscara de rede, usado para alocação de endereços.

..
	The next two lines of code in our example script, ``first.cc``,

As próximas duas linhas de código,

::

    Ipv4AddressHelper address;
    address.SetBase ("10.1.1.0", "255.255.255.0");

..
	declare an address helper object and tell it that it should begin allocating IP
	addresses from the network 10.1.1.0 using the mask 255.255.255.0 to define 
	the allocatable bits.  By default the addresses allocated will start at one
	and increase monotonically, so the first address allocated from this base will
	be 10.1.1.1, followed by 10.1.1.2, etc.  The low level |ns3| system
	actually remembers all of the IP addresses allocated and will generate a
	fatal error if you accidentally cause the same address to be generated twice 
	(which is a very hard to debug error, by the way).

declara um assistente de endereçamento e diz para ele iniciar a alocação de IP's na rede 10.1.1.0 usando a máscara 255.255.255.0. Por padrão, os endereços alocados irão iniciar do primeiro endereço IP disponível e serão incrementados um a um. Então, o primeiro endereço IP alocado será o 10.1.1.1, seguido pelo 10.1.1.2, etc. Em um nível mais baixo, o |ns3| mantém todos os endereços IP's alocados e gera um erro fatal se você acidentalmente usar o mesmo endereço duas vezes (esse é um erro muito difícil de depurar).

..
	The next line of code,

A próxima linha de código,

::

    Ipv4InterfaceContainer interfaces = address.Assign (devices);

..
	performs the actual address assignment.  In |ns3| we make the
	association between an IP address and a device using an ``Ipv4Interface``
	object.  Just as we sometimes need a list of net devices created by a helper 
	for future reference we sometimes need a list of ``Ipv4Interface`` objects.
	The ``Ipv4InterfaceContainer`` provides this functionality.

realiza efetivamente o endereçamento. No |ns3| nós fazemos a associação entre endereços IP e dispositivos usando um objeto ``Ipv4Interface``. As vezes precisamos de uma lista dos dispositivos de rede criados pelo assistente de topologia para consultas futuras. O ``Ipv4InterfaceContainer`` fornece esta funcionalidade.

..
	Now we have a point-to-point network built, with stacks installed and IP 
	addresses assigned.  What we need at this point are applications to generate
	traffic.

Agora que nós temos uma rede ponto-a-ponto funcionando, com pilhas de protocolos instaladas e endereços IP's configurados. O que nós precisamos são aplicações para gerar o tráfego de rede.

.. 
	Applications

Aplicações
++++++++++

..
	Another one of the core abstractions of the ns-3 system is the 
	``Application``.  In this script we use two specializations of the core
	|ns3| class ``Application`` called ``UdpEchoServerApplication``
	and ``UdpEchoClientApplication``.  Just as we have in our previous 
	explanations,  we use helper objects to help configure and manage the 
	underlying objects.  Here, we use ``UdpEchoServerHelper`` and
	``UdpEchoClientHelper`` objects to make our lives easier.

Outra abstração do núcleo do |ns3| são as aplicações (``Application``). Neste código são utilizadas duas especializações da classe ``Application`` chamadas ``UdpEchoServerApplication`` e  ``UdpEchoClientApplication``. Assim como nas explicações anteriores que nós utilizamos assistentes para configurar e gerenciar outros objetos, nós usaremos os objetos ``UdpEchoServerHelper`` e ``UdpEchoClientHelper`` para deixar a nossa vida mais fácil.

UdpEchoServerHelper
~~~~~~~~~~~~~~~~~~~
..
	The following lines of code in our example script, ``first.cc``, are used
	to set up a UDP echo server application on one of the nodes we have previously
	created.

As linhas seguintes do código do exemplo ``first.cc``, são usadas para configurar uma aplicação de eco (`echo`) UDP em um dos nós criados anteriormente.

::

    UdpEchoServerHelper echoServer (9);

    ApplicationContainer serverApps = echoServer.Install (nodes.Get (1));
    serverApps.Start (Seconds (1.0));
    serverApps.Stop (Seconds (10.0));

..
	The first line of code in the above snippet declares the 
	``UdpEchoServerHelper``.  As usual, this isn't the application itself, it
	is an object used to help us create the actual applications.  One of our 
	conventions is to place *required* ``Attributes`` in the helper constructor.
	In this case, the helper can't do anything useful unless it is provided with
	a port number that the client also knows about.  Rather than just picking one 
	and hoping it all works out, we require the port number as a parameter to the 
	constructor.  The constructor, in turn, simply does a ``SetAttribute``
	with the passed value.  If you want, you can set the "Port" ``Attribute``
	to another value later using ``SetAttribute``.

Um fragmento da primeira linha do código declara o ``UdpEchoServerHelper``. Esta não é a própria aplicação, é o objeto usado para ajudar na criação da aplicação. Uma de nossas convenções é colocar os atributos *obrigatórios* no construtor do assistente de topologia. Neste caso, o assistente não pode fazer nada se não colocarmos um número de porta que o cliente conhece. O construtor, por sua vez, configura o atributo "Port" usando ``SetAttribute``.

..
	Similar to many other helper objects, the ``UdpEchoServerHelper`` object 
	has an ``Install`` method.  It is the execution of this method that actually
	causes the underlying echo server application to be instantiated and attached
	to a node.  Interestingly, the ``Install`` method takes a
	``NodeContainter`` as a parameter just as the other ``Install`` methods
	we have seen.  This is actually what is passed to the method even though it 
	doesn't look so in this case.  There is a C++ *implicit conversion* at
	work here that takes the result of ``nodes.Get (1)`` (which returns a smart
	pointer to a node object --- ``Ptr<Node>``) and uses that in a constructor
	for an unnamed ``NodeContainer`` that is then passed to ``Install``.
	If you are ever at a loss to find a particular method signature in C++ code
	that compiles and runs just fine, look for these kinds of implicit conversions.  

De forma semelhante aos outros assistentes, o ``UdpEchoServerHelper`` tem o método ``Install``. É este método que instancia a aplicação de servidor de eco (``echo server``) e a associa ao nó. O método ``Install`` tem como parâmetro um ``NodeContainter``, assim como o outro método ``Install`` visto. Isto é o que é passado para o método, mesmo que não seja visível. Há uma *conversão implícita* em C++, que pega o resultado de ``nodes.Get (1)`` (o qual retorna um ponteiro para o objeto `node` --- ``Ptr<Node>``) e o usa em um construtor de um ``NodeContainer`` sem nome, que então é passado para o método ``Install``.

..
	We now see that ``echoServer.Install`` is going to install a
	``UdpEchoServerApplication`` on the node found at index number one of the
	``NodeContainer`` we used to manage our nodes.  ``Install`` will return
	a container that holds pointers to all of the applications (one in this case 
	since we passed a ``NodeContainer`` containing one node) created by the 
	helper.

Agora vemos que o ``echoServer.Install`` instala um ``UdpEchoServerApplication`` no primeiro nó do ``NodeContainer``. O ``Install`` irá retornar um contêiner que armazena os ponteiros de todas as aplicações (neste caso passamos um ``NodeContainer`` contendo um nó) criadas pelo assistente de topologia.

..
	Applications require a time to "start" generating traffic and may take an
	optional time to "stop".  We provide both.  These times are set using  the
	``ApplicationContainer`` methods ``Start`` and ``Stop``.  These 
	methods take ``Time`` parameters.  In this case, we use an *explicit*
	C++ conversion sequence to take the C++ double 1.0 and convert it to an 
	|ns3| ``Time`` object using a ``Seconds`` cast.  Be aware that
	the conversion rules may be controlled by the model author, and C++ has its
	own rules, so you can't always just assume that parameters will be happily 
	converted for you.  The two lines,

As aplicações requerem um tempo para "iniciar" a geração de tráfego de rede e podem ser opcionalmente "desligadas".  Estes tempos podem ser configurados usando o ``ApplicationContainer`` com os métodos ``Start`` e ``Stop``, respectivamente. Esses métodos possuem o parâmetro ``Time``. Em nosso exemplo, nós usamos uma convenção explicita do C++ para passar 1.0 e converter em um objeto ``Time`` usando segundos. Esteja ciente que as regras de conversão podem ser controladas pelo autor do modelo e o C++ tem suas próprias regras, desta forma, você não pode assumir que o parâmetro sempre vai ser convertido para você. As duas linhas, 

::

    serverApps.Start (Seconds (1.0));
    serverApps.Stop (Seconds (10.0));

..
	will cause the echo server application to ``Start`` (enable itself) at one
	second into the simulation and to ``Stop`` (disable itself) at ten seconds
	into the simulation.  By virtue of the fact that we have declared a simulation
	event (the application stop event) to be executed at ten seconds, the simulation
	will last *at least* ten seconds.

irão iniciar (``Start``) a aplicação de servidor de eco um segundo após o início da simulação e depois desligar (``Stop``) em dez segundos. Em virtude de termos declarado que um evento de simulação (o evento de desligamento da aplicação) deve ser executado por dez segundos, a simulação vai durar pelo menos dez segundos.

UdpEchoClientHelper
~~~~~~~~~~~~~~~~~~~

..
	The echo client application is set up in a method substantially similar to
	that for the server.  There is an underlying ``UdpEchoClientApplication``
	that is managed by an ``UdpEchoClientHelper``.

A aplicação cliente de eco é configurada de forma muito similar ao servidor. Há o ``UdpEchoClientApplication`` que é gerenciado por um ``UdpEchoClientHelper``.

::

    UdpEchoClientHelper echoClient (interfaces.GetAddress (1), 9);
    echoClient.SetAttribute ("MaxPackets", UintegerValue (1));
    echoClient.SetAttribute ("Interval", TimeValue (Seconds (1.)));
    echoClient.SetAttribute ("PacketSize", UintegerValue (1024));

    ApplicationContainer clientApps = echoClient.Install (nodes.Get (0));
    clientApps.Start (Seconds (2.0));
    clientApps.Stop (Seconds (10.0));

..
	For the echo client, however, we need to set five different ``Attributes``.
	The first two ``Attributes`` are set during construction of the 
	``UdpEchoClientHelper``.  We pass parameters that are used (internally to
	the helper) to set the "RemoteAddress" and "RemotePort" ``Attributes``
	in accordance with our convention to make required ``Attributes`` parameters
	in the helper constructors. 

Para o cliente de eco, precisamos configurar cinco diferentes atributos. Os dois primeiros são configurados durante a construção do ``UdpEchoClientHelper``. Passamos os parâmetros que são usados (internamente pelo Assistente) para configurar os atributos "RemoteAddress" (endereço remoto) e "RemotePort" (porta remota).

..
	Recall that we used an ``Ipv4InterfaceContainer`` to keep track of the IP 
	addresses we assigned to our devices.  The zeroth interface in the 
	``interfaces`` container is going to correspond to the IP address of the 
	zeroth node in the ``nodes`` container.  The first interface in the 
	``interfaces`` container corresponds to the IP address of the first node 
	in the ``nodes`` container.  So, in the first line of code (from above), we
	are creating the helper and telling it so set the remote address of the client
	to be  the IP address assigned to the node on which the server resides.  We 
	also tell it to arrange to send packets to port nine.

Lembre-se que usamos um ``Ipv4InterfaceContainer`` para configurar o endereço IP em nossos dispositivos. A interface zero (primeira) no contêiner corresponde ao endereço IP do nó zero no contêiner de nós. A primeira interface corresponde ao endereço IP do primeiro nó. Então, na primeira linha do código anterior, nós criamos um assistente e dizemos ao nó para configurar o endereço remoto do cliente conforme o IP do servidor. Nós dizemos também para enviar pacotes para a porta nove.

..
	The "MaxPackets" ``Attribute`` tells the client the maximum number of 
	packets we allow it to send during the simulation.  The "Interval" 
	``Attribute`` tells the client how long to wait between packets, and the
	"PacketSize" ``Attribute`` tells the client how large its packet payloads
	should be.  With this particular combination of ``Attributes``, we are 
	telling the client to send one 1024-byte packet.

O atributo "MaxPackets" diz ao cliente o número máximo de pacotes que são permitidos para envio durante a simulação. O atributo "Interval" diz ao cliente quanto tempo esperar entre os pacotes e o "PacketSize" informa ao cliente qual é o tamanho da área de dados do pacote. Com esta combinação de atributos que nós fizemos teremos clientes enviando pacotes de 1024 bytes.

..
	Just as in the case of the echo server, we tell the echo client to ``Start``
	and ``Stop``, but here we start the client one second after the server is
	enabled (at two seconds into the simulation).

Assim como no caso do servidor de eco, nós dizemos para o cliente de eco iniciar e parar, mas aqui nós iniciamos o cliente um segundo depois que o servidor estiver funcionando (com dois segundos de simulação).

.. Simulator

Simulador (`Simulator`)
+++++++++++++++++++++++

..
	What we need to do at this point is to actually run the simulation.  This is 
	done using the global function ``Simulator::Run``.

O que nós precisamos agora é executar o simulador. Isto é feito usando a função global ``Simulator::Run``.

::

    Simulator::Run ();

..
	When we previously called the methods,

Quando nós chamamos os métodos

::

    serverApps.Start (Seconds (1.0));
    serverApps.Stop (Seconds (10.0));
    ...
    clientApps.Start (Seconds (2.0));
    clientApps.Stop (Seconds (10.0));

..
	we actually scheduled events in the simulator at 1.0 seconds, 2.0 seconds and
	two events at 10.0 seconds.  When ``Simulator::Run`` is called, the system 
	will begin looking through the list of scheduled events and executing them.  
	First it will run the event at 1.0 seconds, which will enable the echo server 
	application (this event may, in turn, schedule many other events).  Then it 
	will run the event scheduled for t=2.0 seconds which will start the echo client
	application.  Again, this event may schedule many more events.  The start event
	implementation in the echo client application will begin the data transfer phase
	of the simulation by sending a packet to the server.

agendamos os eventos no simulador em 1 segundo, 2 segundos e dois eventos em 10 segundos. Quando chamamos ``Simulator::Run``, o sistema verificará a lista de eventos agendados e os executará no momento apropriado. Primeiramente, ele vai executar o evento de 1 segundo que inicia a aplicação de servidor de eco. Depois executa o evento agendado com dois segundos (t=2,0) que iniciará a aplicação do cliente de eco. Estes eventos podem agendar muitos outros eventos. O evento `start` do cliente irá iniciar a fase de transferência de dados na simulação enviando pacotes ao servidor.

..
	The act of sending the packet to the server will trigger a chain of events
	that will be automatically scheduled behind the scenes and which will perform 
	the mechanics of the packet echo according to the various timing parameters 
	that we have set in the script.

O ato de enviar pacotes para o servidor vai disparar uma cadeia de eventos que serão automaticamente escalonados e executarão a mecânica do envio de pacotes de eco de acordo com os vários parâmetros de tempo que configuramos no código.

..
	Eventually, since we only send one packet (recall the ``MaxPackets`` 
	``Attribute`` was set to one), the chain of events triggered by 
	that single client echo request will taper off and the simulation will go 
	idle.  Once this happens, the remaining events will be the ``Stop`` events
	for the server and the client.  When these events are executed, there are
	no further events to process and ``Simulator::Run`` returns.  The simulation
	is then complete.

Considerando que enviamos somente um pacote (lembre-se que o atributo ``MaxPackets`` foi definido com um), uma cadeia de eventos será disparada por este único pedido de eco do cliente até cessar e o simulador ficará ocioso. Uma vez que isto ocorra, os eventos restantes serão o ``Stop`` do servidor e do cliente. Quando estes eventos forem executados, não havendo mais eventos para processar, o ``Simulator::Run`` retorna. A simulação está completa.

..
	All that remains is to clean up.  This is done by calling the global function 
	``Simulator::Destroy``.  As the helper functions (or low level 
	|ns3| code) executed, they arranged it so that hooks were inserted in
	the simulator to destroy all of the objects that were created.  You did not 
	have to keep track of any of these objects yourself --- all you had to do 
	was to call ``Simulator::Destroy`` and exit.  The |ns3| system
	took care of the hard part for you.  The remaining lines of our first 
	|ns3| script, ``first.cc``, do just that:

Tudo que resta é limpar. Isto é feito chamando uma função global chamada ``Simulator::Destroy``. Uma das funções dos assistentes (ou do código de baixo nível do |ns3|) é agrupar todos os objetos que foram criados e destruí-los. Você não precisa tratar estes objetos --- tudo que precisa fazer é chamar ``Simulator::Destroy`` e sair. O |ns3| cuidará desta difícil tarefa para você. As linhas restantes do código fazem isto:

::

    Simulator::Destroy ();
    return 0;
  }

.. Building Your Script

Construindo o código
++++++++++++++++++++

..
	We have made it trivial to build your simple scripts.  All you have to do is 
	to drop your script into the scratch directory and it will automatically be 
	built if you run Waf.  Let's try it.  Copy ``examples/tutorial/first.cc`` into 
	the ``scratch`` directory after changing back into the top level directory.

É trivial construir (criar os binários de) seu código. Tudo que tem a fazer é copiar seu código para dentro do diretório ``scratch`` e ele será construído automaticamente quando executar o Waf. Copie ``examples/tutorial/first.cc`` para o diretório ``scratch``  e depois volte ao diretório principal.

::

  cd ../..
  cp examples/tutorial/first.cc scratch/myfirst.cc

..
	Now build your first example script using waf:

Agora construa seu primeiro exemplo usando o Waf:

::

  ./waf

..
	You should see messages reporting that your ``myfirst`` example was built
	successfully.

Você deve ver mensagens reportando que o seu exemplo ``myfirst`` foi construído com sucesso.

::

  Waf: Entering directory `/home/craigdo/repos/ns-3-allinone/ns-3-dev/build'
  [614/708] cxx: scratch/myfirst.cc -> build/debug/scratch/myfirst_3.o
  [706/708] cxx_link: build/debug/scratch/myfirst_3.o -> build/debug/scratch/myfirst
  Waf: Leaving directory `/home/craigdo/repos/ns-3-allinone/ns-3-dev/build'
  'build' finished successfully (2.357s)

..
	You can now run the example (note that if you build your program in the scratch
	directory you must run it out of the scratch directory):

Você agora pode executar o exemplo (note que se você construiu seu programa no diretório ``scratch``, então deve executar o comando fora deste diretório):

::

  ./waf --run scratch/myfirst

..
	You should see some output:

Você deverá ver algumas saídas:

::

  Waf: Entering directory `/home/craigdo/repos/ns-3-allinone/ns-3-dev/build'
  Waf: Leaving directory `/home/craigdo/repos/ns-3-allinone/ns-3-dev/build'
  'build' finished successfully (0.418s)
  Sent 1024 bytes to 10.1.1.2
  Received 1024 bytes from 10.1.1.1
  Received 1024 bytes from 10.1.1.2

..
	Here you see that the build system checks to make sure that the file has been
	build and then runs it.  You see the logging component on the echo client 
	indicate that it has sent one 1024 byte packet to the Echo Server on 
	10.1.1.2.  You also see the logging component on the echo server say that
	it has received the 1024 bytes from 10.1.1.1.  The echo server silently 
	echoes the packet and you see the echo client log that it has received its 
	packet back from the server.

O sistema verifica se os arquivos foram construídos e então executa-os. Através do componente de registro vemos que o cliente enviou 1024 bytes para o servidor através do IP 10.1.1.2. Também podemos ver que o servidor diz ter recebido 1024 bytes do IP 10.1.1.1 e ecoa o pacote para o cliente, que registra o seu recebimento.

.. Ns-3 Source Code

Código fonte do Ns-3
********************

.. 
	Now that you have used some of the |ns3| helpers you may want to 
	have a look at some of the source code that implements that functionality.
	The most recent code can be browsed on our web server at the following link:
	http://code.nsnam.org/ns-3-dev.  There, you will see the Mercurial
	summary page for our |ns3| development tree.

Agora que você já utilizou alguns assistentes do |ns3|, podemos dar uma olhada no código fonte que implementa estas funcionalidades. Pode-se navegar o código mais recente no seguinte endereço: http://code.nsnam.org/ns-3-dev. Lá você verá a página de sumário do Mercurial para a árvore de desenvolvimento do |ns3|.

..
	At the top of the page, you will see a number of links,

No início da página, você verá vários `links`,

::

  summary | shortlog | changelog | graph | tags | files 

..
	Go ahead and select the ``files`` link.  This is what the top-level of
	most of our *repositories* will look:

selecione ``files``. Aparecerá o primeiro nível do repositório:

::

  drwxr-xr-x                               [up]     
  drwxr-xr-x                               bindings python  files
  drwxr-xr-x                               doc              files
  drwxr-xr-x                               examples         files
  drwxr-xr-x                               ns3              files
  drwxr-xr-x                               scratch          files
  drwxr-xr-x                               src              files
  drwxr-xr-x                               utils            files
  -rw-r--r-- 2009-07-01 12:47 +0200 560    .hgignore        file | revisions | annotate
  -rw-r--r-- 2009-07-01 12:47 +0200 1886   .hgtags          file | revisions | annotate
  -rw-r--r-- 2009-07-01 12:47 +0200 1276   AUTHORS          file | revisions | annotate
  -rw-r--r-- 2009-07-01 12:47 +0200 30961  CHANGES.html     file | revisions | annotate
  -rw-r--r-- 2009-07-01 12:47 +0200 17987  LICENSE          file | revisions | annotate
  -rw-r--r-- 2009-07-01 12:47 +0200 3742   README           file | revisions | annotate
  -rw-r--r-- 2009-07-01 12:47 +0200 16171  RELEASE_NOTES    file | revisions | annotate
  -rw-r--r-- 2009-07-01 12:47 +0200 6      VERSION          file | revisions | annotate
  -rwxr-xr-x 2009-07-01 12:47 +0200 88110  waf              file | revisions | annotate
  -rwxr-xr-x 2009-07-01 12:47 +0200 28     waf.bat          file | revisions | annotate
  -rw-r--r-- 2009-07-01 12:47 +0200 35395  wscript          file | revisions | annotate
  -rw-r--r-- 2009-07-01 12:47 +0200 7673   wutils.py        file | revisions | annotate
  
..
	Our example scripts are in the ``examples`` directory.  If you click on ``examples``
	you will see a list of subdirectories.  One of the files in ``tutorial`` subdirectory is ``first.cc``.  If
	you click on ``first.cc`` you will find the code you just walked through.

Os códigos exemplo estão no diretório ``examples``. Se você clicar verá uma lista de subdiretórios. Um dos arquivos no subdiretório ``tutorial`` é o ``first.cc``. Clicando nele você encontrará o código que acabamos de analisar.

..
	The source code is mainly in the ``src`` directory.  You can view source
	code either by clicking on the directory name or by clicking on the ``files``
	link to the right of the directory name.  If you click on the ``src``
	directory, you will be taken to the listing of the ``src`` subdirectories.  If you 
	then click on ``core`` subdirectory, you will find a list of files.  The first file
	you will find (as of this writing) is ``abort.h``.  If you click on the 
	``abort.h`` link, you will be sent to the source file for ``abort.h`` which 
	contains useful macros for exiting scripts if abnormal conditions are detected.


O código fonte é mantido no diretório ``src``. Você pode vê-lo clicando sobre o nome do diretório ou clicando no item ``files`` a direita do nome. Clicando no diretório ``src``, obterá uma lista de subdiretórios. Clicando no subdiretório ``core``, encontrará um lista de arquivos. O primeiro arquivo é o ``abort.h``, que contém macros caso condições anormais sejam encontradas.

..
	The source code for the helpers we have used in this chapter can be found in the 
	``src/applications/helper`` directory.  Feel free to poke around in the directory tree to
	get a feel for what is there and the style of |ns3| programs.

O código fonte para os assistentes utilizados neste capítulo podem ser encontrados no diretório ``src/applications/helper``. Sinta-se à vontade para explorar a árvore de diretórios e ver o estilo de código do |ns3|.

