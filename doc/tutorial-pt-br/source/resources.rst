.. include:: replace.txt

..
	========================================================================================
	Translated for portuguese by the students of the inter-institutional doctorate program of IME-USP/UTFPR-CM.
	
	Traduzido para o português pelos alunos do programa de doutorado inter institucional do Instituto de Matemática e Estatística da Universidade de São Paulo --- IME-USP em parceria com a Universidade Tecnológica Federal do Paraná - Campus Campo Mourão --- UTFPR-CM:
	
	* Frank Helbert (frank@ime.usp.br);
	* Luiz Arthur Feitosa dos Santos (luizsan@ime.usp.br);
	* Rodrigo Campiolo (campiolo@ime.usp.br).
	========================================================================================


.. Resources

Recursos
---------

.. The Web

A Internet
**********

..
	There are several important resources of which any |ns3| user must be
	aware.  The main web site is located at http://www.nsnam.org and 
	provides access to basic information about the |ns3| system.  Detailed 
	documentation is available through the main web site at
	http://www.nsnam.org/documentation/.  You can also find documents 
	relating to the system architecture from this page.

Há vários recursos importantes que um usuário do |ns3| deve conhecer. O principal está em http://www.nsnam.org e fornece acesso a informações básicas sobre o |ns3|. A documentação detalhada esta disponível no sítio principal através do endereço http://www.nsnam.org/documentation/. Nesta página, também podem ser encontrados documentos relacionados a arquitetura do sistema.

..
	There is a Wiki that complements the main |ns3| web site which you will
	find at http://www.nsnam.org/wiki/.  You will find user and developer 
	FAQs there, as well as troubleshooting guides, third-party contributed code, 
	papers, etc. 

Também existe um *Wiki* que completa o sítio do |ns3| e pode ser encontrado em http://www.nsnam.org/wiki/. Nesta página são encontradas perguntas freqüentes - FAQs (do inglês, *Frequently Asked Questions*) para usuários e desenvolvedores, guias para resolução de problemas, código de terceiros, artigos, etc.

..
	The source code may be found and browsed at http://code.nsnam.org/. 
	There you will find the current development tree in the repository named
	``ns-3-dev``. Past releases and experimental repositories of the core
	developers may also be found there.

O código fonte também pode ser encontrado e explorado em http://code.nsnam.org/. Neste encontra-se a árvore de código em desenvolvimento em um repositório chamado ``ns-3-dev``. Repositórios antigos e experimentais do núcleo de desenvolvimento podem ser encontrados neste sítio também.

Mercurial
*********

..
	Complex software systems need some way to manage the organization and 
	changes to the underlying code and documentation.  There are many ways to
	perform this feat, and you may have heard of some of the systems that are
	currently used to do this.  The Concurrent Version System (CVS) is probably
	the most well known.

Sistemas complexos precisam gerenciar a organização e alterações do código, bem como a documentação. Existem várias maneiras de fazer isto e o leitor provavelmente já ouviu falar de algumas. O *Concurrent Version System (CVS)* --- em português, Sistema de Versões Concorrentes --- é provavelmente o mais conhecido.

..
	The |ns3| project uses Mercurial as its source code management system.
	Although you do not need to know much about Mercurial in order to complete
	this tutorial, we recommend becoming familiar with Mercurial and using it 
	to access the source code.  Mercurial has a web site at 
	http://www.selenic.com/mercurial/,
	from which you can get binary or source releases of this Software
	Configuration Management (SCM) system.  Selenic (the developer of Mercurial)
	also provides a tutorial at 
	http://www.selenic.com/mercurial/wiki/index.cgi/Tutorial/,
	and a QuickStart guide at
	http://www.selenic.com/mercurial/wiki/index.cgi/QuickStart/.

O |ns3| utiliza o Mercurial para isto. Embora não seja necessário conhecer muito sobre o Mercurial para entender este tutorial, recomenda-se a familiarização com o uso da ferramenta para acessar o código fonte do sistema. O Mercurial tem um sítio em http://www.selenic.com/mercurial/, no qual pode-se baixar diretamente os executáveis ou o código fonte deste sistema de *Software Configuration Management (SCM)* --- em português, Software de Gerenciamento de Configuração. Selenic (o desenvolvedor do Mercurial), também fornece tutoriais em http://www.selenic.com/mercurial/wiki/index.cgi/Tutorial/, e um guia rápido em http://www.selenic.com/mercurial/wiki/index.cgi/QuickStart/.

..
	You can also find vital information about using Mercurial and |ns3|
	on the main |ns3| web site.

Informações vitais de como usar o Mercurial e o |ns3| são encontradas no sítio principal do projeto.

Waf
***

..
	Once you have source code downloaded to your local system, you will need 
	to compile that source to produce usable programs.  Just as in the case of
	source code management, there are many tools available to perform this 
	function.  Probably the most well known of these tools is ``make``.  Along
	with being the most well known, ``make`` is probably the most difficult to
	use in a very large and highly configurable system.  Because of this, many
	alternatives have been developed.  Recently these systems have been developed
	using the Python language.

Uma vez baixado o código fonte para o seu sistema de arquivos local, será necessário compilar estes fontes para criar os executáveis. Para esta tarefa existem várias ferramentas disponíveis. Provavelmente a mais conhecida é o Make. Além de mais conhecido, também deve ser o mais difícil de usar em grandes sistemas e com muitas opções de configuração. Por este motivo, muitas alternativas foram desenvolvidas, utilizando principalmente a linguagem Python.

..
	The build system Waf is used on the |ns3| project.  It is one 
	of the new generation of Python-based build systems.  You will not need to 
	understand any Python to build the existing |ns3| system, and will 
	only have to understand a tiny and intuitively obvious subset of Python in 
	order to extend the system in most cases.

O Waf é utilizado para gerar os binários no projeto |ns3|. Ele faz parte da nova geração de sistemas de compilação e contrução baseados em Python. O leitor não precisa entender nada de Python para compilar o |ns3|, e terá que entender um pequeno e intuitivo subconjunto da linguagem se quiser estender o sistema.

..
	For those interested in the gory details of Waf, the main web site can be 
	found at http://code.google.com/p/waf/.

Para os interessados em mais detalhes sobre o Waf, basta acessar o sítio http://code.google.com/p/waf/.

.. Development Environment

Ambiente de Desenvolvimento
***************************

..
	As mentioned above, scripting in |ns3| is done in C++ or Python.
	As of ns-3.2, most of the |ns3| API is available in Python, but the 
	models are written in C++ in either case.  A working 
	knowledge of C++ and object-oriented concepts is assumed in this document.
	We will take some time to review some of the more advanced concepts or 
	possibly unfamiliar language features, idioms and design patterns as they 
	appear.  We don't want this tutorial to devolve into a C++ tutorial, though,
	so we do expect a basic command of the language.  There are an almost 
	unimaginable number of sources of information on C++ available on the web or
	in print.

Como mencionado anteriormente, a programação no |ns3| é feita em C++ ou Python. A partir do ns-3.2, a maioria das APIs já estão disponíveis em Python, mas os modelos continuam sendo escritos em C++. Considera-se que o leitor possui conhecimento básico de C++ e conceitos de orientação a objetos neste documento. Somente serão revistos conceitos avançados, possíveis características pouco utilizadas da linguagem, dialetos e padrões de desenvolvimento. O objetivo não é tornar este um tutorial de C++, embora seja necessário saber o básico da linguagem. Para isto, existe um número muito grande de fontes de informação na Web e em materiais impressos (livros, tutoriais, revistas, etc).

..
	If you are new to C++, you may want to find a tutorial- or cookbook-based
	book or web site and work through at least the basic features of the language
	before proceeding.  For instance, `this tutorial
	<http://www.cplusplus.com/doc/tutorial/>`_.

Se você é inexperiente em C++, pode encontrar tutoriais, livros e sítios Web para obter o mínimo de conhecimento sobre a linguagem antes de continuar. Por exemplo, pode utilizar `este tutorial <http://www.cplusplus.com/doc/tutorial/>`_.

..
	The |ns3| system uses several components of the GNU "toolchain" 
	for development.  A 
	software toolchain is the set of programming tools available in the given 
	environment. For a quick review of what is included in the GNU toolchain see,
	http://en.wikipedia.org/wiki/GNU_toolchain.  |ns3| uses gcc, 
	GNU binutils, and gdb.  However, we do not use the GNU build system tools, 
	neither make nor autotools.  We use Waf for these functions.

O |ns3| utiliza vários componentes do conjunto de ferramentas GNU --- `"GNU toolchain"` --- para o desenvolvimento. Um `software toolchain` é um conjunto de ferramentas de programação para um determinado ambiente. Para uma breve visão do que consiste o `GNU toolchain` veja http://en.wikipedia.org/wiki/GNU_toolchain. O |ns3| usa o `gcc`, `GNU binutils` e `gdb`. Porém, não usa as ferramentas GNU para compilar o sistema, nem o Make e nem o Autotools. Para estas funções é utilizado o Waf.

..
	Typically an |ns3| author will work in Linux or a Linux-like
	environment.  For those running under Windows, there do exist environments 
	which simulate the Linux environment to various degrees.  The |ns3| 
	project supports development in the Cygwin environment for 
	these users.  See http://www.cygwin.com/ 
	for details on downloading (MinGW is presently not officially supported,
	although some of the project maintainers to work with it). Cygwin provides 
	many of the popular Linux system commands.  It can, however, sometimes be 
	problematic due to the way it actually does its emulation, and sometimes
	interactions with other Windows software can cause problems.

Normalmente um usuário do |ns3| irá trabalhar no Linux ou um ambiente baseado nele. Para aqueles que usam Windows, existem ambientes que simulam o Linux em vários níveis. Para estes usuários, o projeto |ns3| fornece suporte ao ambiente Cygwin. Veja o sítio http://www.cygwin.com/ para detalhes de como baixá-lo (o MinGW não é suportado oficialmente, embora alguns mantenedores do projeto trabalhem com ele). O Cygwin fornece vários comandos populares do Linux, entretanto podemos ter problemas com a emulação, às vezes a interação com outros programas do Windows pode causar problemas.

..
	If you do use Cygwin or MinGW; and use Logitech products, we will save you
	quite a bit of heartburn right off the bat and encourage you to take a look
	at the `MinGW FAQ
	<http://oldwiki.mingw.org/index.php/FAQ>`_.

Se você usa o Cygwin ou MinGW e usa produtos da Logitech, evite dores de cabeça e dê uma olhada em `MinGW FAQ	<http://oldwiki.mingw.org/index.php/FAQ>`_.

..
	Search for "Logitech" and read the FAQ entry, "why does make often 
	crash creating a sh.exe.stackdump file when I try to compile my source code."
	Believe it or not, the ``Logitech Process Monitor`` insinuates itself into
	every DLL in the system when it is running.  It can cause your Cygwin or
	MinGW DLLs to die in mysterious ways and often prevents debuggers from 
	running.  Beware of Logitech software when using Cygwin.

Busque por "Logitech" e leia a entrada com o assunto: "why does make often crash creating a sh.exe.stackdump file when I try to compile my source code.". Acredite ou não, o ``Logitech Process Monitor`` influencia todas as DLLs do sistema. Isto pode ocasionar problemas misteriosos durante a execução do Cygwin ou do MinGW. Muita cautela quando utilizar software da Logitech junto com o Cygwin.

..
	Another alternative to Cygwin is to install a virtual machine environment
	such as VMware server and install a Linux virtual machine.


Uma alternativa ao Cygwin é instalar um ambiente de máquina virtual, tal como o VMware server e criar uma máquina virtual Linux.


.. Socket Programming

Programando com Soquetes (Sockets)
**********************************

..
	We will assume a basic facility with the Berkeley Sockets API in the examples
	used in this tutorial.  If you are new to sockets, we recommend reviewing the
	API and some common usage cases.  For a good overview of programming TCP/IP
	sockets we recommend `TCP/IP Sockets in C, Donahoo and Calvert
	<http://www.elsevier.com/wps/find/bookdescription.
	cws_home/717656/description#description>`_.


Neste tutorial assume-se, nos exemplos utilizados, que o leitor está familiarizado com as funcionalidades básicas da API dos soquetes de Berkeley. Se este não for o caso, recomendamos a leitura das APIs e alguns casos de uso comuns. Uma API --- do Inglês, `Application Programming Interface` --- é um é um conjunto de rotinas e padrões estabelecidos por um software para a utilização das suas funcionalidades. Para uma boa visão geral sobre a programação de soquetes TCP/IP sugerimos `TCP/IP Sockets in C, Donahoo and Calvert
<http://www.elsevier.com/wps/find/bookdescription.cws_home/717656/description#description>`_.

..
	There is an associated web site that includes source for the examples in the
	book, which you can find at:
	http://cs.baylor.edu/~donahoo/practical/CSockets/.

O sítio http://cs.baylor.edu/~donahoo/practical/CSockets/ contém os códigos fontes dos exemplos do livro.

..
	If you understand the first four chapters of the book (or for those who do
	not have access to a copy of the book, the echo clients and servers shown in 
	the website above) you will be in good shape to understand the tutorial.
	There is a similar book on Multicast Sockets,
	`Multicast Sockets, Makofske and Almeroth
	<http://www.elsevier.com/wps/find/bookdescription.cws_home/700736/description#description>`_.
	that covers material you may need to understand if you look at the multicast 
	examples in the distribution.

Se o leitor entender os primeiros quatro capítulos do livro (ou para aqueles que não têm acesso ao livro, os exemplos de cliente e servidor de eco mostrado no sítio anterior) estará apto para compreender o tutorial. Existe também um livro sobre soquetes multidifusão, `Multicast Sockets, Makofske and Almeroth	<http://www.elsevier.com/wps/find/bookdescription.cws_home/700736/description#description>`_. que é um material que cobre o necessário sobre multidifusão caso o leitor se interesse.

