.. include:: replace.txt

..
	========================================================================================
	Translated for portuguese by the students of the inter-institutional doctorate program of IME-USP/UTFPR-CM.
	
	Traduzido para o português pelos alunos do programa de doutorado inter institucional do Instituto de Matemática e Estatística da Universidade de São Paulo --- IME-USP em parceria com a Universidade Tecnológica Federal do Paraná - Campus Campo Mourão --- UTFPR-CM:
	
	* Frank Helbert (frank@ime.usp.br);
	* Luiz Arthur Feitosa dos Santos (luizsan@ime.usp.br);
	* Rodrigo Campiolo (campiolo@ime.usp.br).
	========================================================================================


.. Getting Started

Iniciando
---------------

.. Downloading ns-3

Baixando o ns-3
****************

.. 
	The |ns3| system as a whole is a fairly complex system and has a
	number of dependencies on other components.  Along with the systems you will
	most likely deal with every day (the GNU toolchain, Mercurial, you programmer
	editor) you will need to ensure that a number of additional libraries are
	present on your system before proceeding.  |ns3| provides a wiki
	for your reading pleasure that includes pages with many useful hints and tips.
	One such page is the "Installation" page,
	http://www.nsnam.org/wiki/Installation. 

O |ns3|, como um todo, é bastante complexo e possui várias dependências. Isto também é verdade para as ferramentas que fornecem suporte ao |ns3| (exemplos, `"GNU toolchain"`, Mercurial e um editor para a programação), desta forma é necessário assegurar que várias bibliotecas estejam presentes no sistema. O |ns3| fornece um Wiki com várias dicas sobre o sistema. Uma das páginas do Wiki é a página de instalação (`"Installation"`) que está disponível em: http://www.nsnam.org/wiki/Installation. 

..
	The "Prerequisites" section of this wiki page explains which packages are 
	required to support common |ns3| options, and also provides the 
	commands used to install them for common Linux variants.  Cygwin users will
	have to use the Cygwin installer (if you are a Cygwin user, you used it to
	install Cygwin). 

A seção de pré-requisitos (`"Prerequisites"`) do Wiki explica quais pacotes são necessários para a instalação básica do |ns3| e também fornece os comandos usados para a instalação nas variantes mais comuns do Linux. Os usuários do Cygwin devem utilizar o ``Cygwin installer``.

..
	You may want to take this opportunity to explore the |ns3| wiki 
	a bit since there really is a wealth of information there. 

Seria interessante explorar um pouco o Wiki, pois lá existe uma grande variedade de informações.


..
	From this point forward, we are going to assume that the reader is working in
	Linux or a Linux emulation environment (Linux, Cygwin, etc.) and has the GNU
	toolchain installed and verified along with the prerequisites mentioned 
	above.  We are also going to assume that you have Mercurial and Waf installed
	and running on the target system.

A partir deste ponto considera-se que o leitor está trabalhando com Linux ou em um ambiente que o emule (Linux, Cygwin, etc), que tenha o `"GNU toolchain"` instalado, bem como os pré-requisitos mencionados anteriormente. Também assume-se que o leitor tenha o Mercurial e o Waf instalados e funcionando em seu sistema.

..
	The |ns3| code is available in Mercurial repositories on the server
	http://code.nsnam.org.  You can also download a tarball release at
	http://www.nsnam.org/releases/, or you can work with repositories
	using Mercurial.  We recommend using Mercurial unless there's a good reason
	not to.  See the end of this section for instructions on how to get a tarball
	release.

Os códigos fonte do |ns3| estão disponíveis através dos repositórios do Mercurial no servidor http://code.nsnam.org. Os fontes compactados podem ser obtidos em http://www.nsnam.org/releases/. No final desta seção há instruções de como obter uma versão compactada. No entanto, a não ser que se tenha uma boa razão, recomenda-se o uso do Mercurial para acesso ao código.

..
	The simplest way to get started using Mercurial repositories is to use the
	``ns-3-allinone`` environment.  This is a set of scripts that manages the 
	downloading and building of various subsystems of |ns3| for you.  We 
	recommend that you begin your |ns3| adventures in this environment
	as it can really simplify your life at this point.

A maneira mais simples de iniciar com o Mercurial é usando o ambiente ``ns-3-allinone``. Trata-se de um conjunto de `scripts` que gerencia o baixar e o construir de vários sub-sistemas do |ns3|. Recomenda-se que os pouco experientes iniciem sua aventura neste ambiente, pois irá realmente facilitar a jornada.

.. Downloading ns-3 Using Mercurial

Obtendo o ns-3 usando o Mercurial
++++++++++++++++++++++++++++++++++
..
	One practice is to create a directory called ``repos`` in one's home 
	directory under which one can keep local Mercurial repositories.  
	*Hint:  we will assume you do this later in the tutorial.*  If you adopt
	that approach, you can get a copy of ``ns-3-allinone`` by typing the 
	following into your Linux shell (assuming you have installed Mercurial):

Para iniciar, uma boa prática é criar um diretório chamado ``repos`` no diretório `home` sobre o qual será mantido o repositório local do Mercurial. *Dica: iremos assumir que o leitor fez isto no restante deste tutorial, então é bom executar este passo!* Se o leitor adotar esta abordagem é possível obter a cópia do ``ns-3-allinone`` executando os comandos a seguir no `shell` Linux (assumindo que o Mercurial está instalado):

::

  cd
  mkdir repos
  cd repos
  hg clone http://code.nsnam.org/ns-3-allinone

..
	As the hg (Mercurial) command executes, you should see something like the 
	following displayed,

Quando executarmos o comando `hg` (Mercurial), teremos como saída algo como:

::

  destination directory: ns-3-allinone
  requesting all changes
  adding changesets
  adding manifests
  adding file changes
  added 31 changesets with 45 changes to 7 files
  7 files updated, 0 files merged, 0 files removed, 0 files unresolved

..
	After the clone command completes, you should have a directory called 
	``ns-3-allinone`` under your ``~/repos`` directory, the contents of which should 
	look something like the following:

Depois que o comando `clone` for executado com sucesso, teremos um diretório chamado ``ns-3-allinone`` dentro do diretório ``~/repos``. O conteúdo deste diretório deve ser algo como:

::

  build.py*  constants.py  dist.py*  download.py*  README  util.py

..
	Notice that you really just downloaded some Python scripts.  The next step
	will be to use those scripts to download and build the |ns3|
	distribution of your choice.

Até agora foram baixados alguns `scripts` em Python. O próximo passo será usar estes `scripts` para baixar e construir a distribuição |ns3| de sua escolha.

..
	If you go to the following link: http://code.nsnam.org/,
	you will see a number of repositories.  Many are the private repositories of
	the |ns3| development team.  The repositories of interest to you will
	be prefixed with "ns-3".  Official releases of |ns3| will be 
	numbered as ``ns-3.<release>.<hotfix>``.  For example, a second hotfix to a
	still hypothetical release forty two of |ns3| would be numbered as
	``ns-3.42.2``.

Acessando o endereço: http://code.nsnam.org/, observa-se vários repositórios. Alguns são privados à equipe de desenvolvimento do |ns3|. Os repositórios de interesse ao leitor estarão prefixados com "ns-3". As `releases` oficiais do |ns3| estarão enumeradas da seguinte forma: ``ns-3.<release>.<hotfix>``. Por exemplo, uma segunda atualização de pequeno porte (`hotfix`) de uma hipotética `release` 42, seria enumerada da seguinte maneira: ``ns-3.42.2``.

..
	The current development snapshot (unreleased) of |ns3| may be found 
	at http://code.nsnam.org/ns-3-dev/.  The 
	developers attempt to keep these repository in consistent, working states but
	they are in a development area with unreleased code present, so you may want 
	to consider staying with an official release if you do not need newly-
	introduced features.

A versão em desenvolvimento (que ainda não é uma `release` oficial) pode ser encontrada em http://code.nsnam.org/ns-3-dev/. Os desenvolvedores tentam manter este repositório em um estado consistente, mas podem existir códigos instáveis. Recomenda-se o uso de uma `release` oficial, a não ser que se necessite das novas funcionalidades introduzidas.

..
	Since the release numbers are going to be changing, I will stick with 
	the more constant ns-3-dev here in the tutorial, but you can replace the 
	string "ns-3-dev" with your choice of release (e.g., ns-3.10) in the 
	text below.  You can find the latest version  of the
	code either by inspection of the repository list or by going to the 
	`"ns-3 Releases"
	<http://www.nsnam.org/releases>`_
	web page and clicking on the latest release link.

Uma vez que o número das versões fica mudando constantemente, neste tutorial será utilizada a versão ns-3-dev, mas o leitor pode escolher outra (por exemplo, ns-3.10). A última versão pode ser encontrada inspecionando a lista de repositórios ou acessando a página `"ns-3 Releases" <http://www.nsnam.org/releases>`_ e clicando em `latest release`.

..
	Go ahead and change into the ``ns-3-allinone`` directory you created when
	you cloned that repository.  We are now going to use the ``download.py`` 
	script to pull down the various pieces of |ns3| you will be using.

Entre no diretório ``ns-3-allinone`` criado anteriormente. O arquivo ``download.py`` será usado para baixar as várias partes do |ns3| que serão utilizadas.

..
	Go ahead and type the following into your shell (remember you can substitute
	the name of your chosen release number instead of ``ns-3-dev`` -- like
	``"ns-3.10"`` if you want to work with a 
	stable release).

Execute os seguintes comandos no `shell` (lembre-se de substituir o número da versão no lugar de ``ns-3-dev`` pela que escolheu, por exemplo, se você optou por usar a décima `release` estável, então deve usar o nome ``"ns-3.10"``).

::

  ./download.py -n ns-3-dev

..
	Note that the default for the ``-n`` option is ``ns-3-dev`` and so the
	above is actually redundant.  We provide this example to illustrate how to
	specify alternate repositories.  In order to download ``ns-3-dev`` you 
	can actually use the defaults and simply type,

O ``ns-3-dev`` é o padrão quando usamos a opção ``-n``, assim o comando poderia ser ``./download.py -n``. O exemplo redundante é apenas para ilustra como especificar repositórios alternativos. Um comando mais simples para obter o ``ns-3-dev`` seria:

::

  ./download.py

..
	As the hg (Mercurial) command executes, you should see something like the 
	following,

Com o comando `hg` (Mercurial) em execução devemos ver a seguinte saída:

::

      #
      # Get NS-3
      #
  
  Cloning ns-3 branch
   =>  hg clone http://code.nsnam.org/ns-3-dev ns-3-dev
  requesting all changes
  adding changesets
  adding manifests
  adding file changes
  added 4634 changesets with 16500 changes to 1762 files
  870 files updated, 0 files merged, 0 files removed, 0 files unresolved

..
	This is output by the download script as it fetches the actual ``ns-3``
	code from the repository.

Esta é a saída do `script` de download obtendo o código atual do repositório ``ns-3``.

..
	The download script is smart enough to know that on some platforms various
	pieces of ns-3 are not supported.  On your platform you may not see some
	of these pieces come down.  However, on most platforms, the process should
	continue with something like,

O `script` de download reconhece que partes do ns-3 não são suportadas na plataforma. Dependendo do sistema, pode ser que a saída não seja exatamente como a mostrada a seguir. Porém, a maioria dos sistemas apresentarão algo como:

::

      #
      # Get PyBindGen
      #

  Required pybindgen version:  0.10.0.640
  Trying to fetch pybindgen; this will fail if no network connection is available.  
  Hit Ctrl-C to skip.
   =>  bzr checkout -rrevno:640 https://launchpad.net/pybindgen pybindgen
  Fetch was successful.

..
	This was the download script getting the Python bindings generator for you.
	Note that you will need bazaar (bzr), a version control system, to download 
	PyBindGen. Next you should see (modulo platform variations) something along 
	the lines of,

Este é o `script` de download obtendo um gerador de `bindings` Python --- um `binding` é literalmente a ligação ou ponte entre dois sistemas, chamaremos aqui de extensões Python. Também será necessário o Bazaar (brz) para baixar o PyBindGen. O Bazaar é um sistema de controle de versões. Em seguida, o leitor deve ver (com algumas variações devido as plataformas) algo parecido com as seguintes linhas:

::

      #
      # Get NSC
      #

  Required NSC version:  nsc-0.5.0
  Retrieving nsc from https://secure.wand.net.nz/mercurial/nsc
   =>  hg clone https://secure.wand.net.nz/mercurial/nsc nsc
  requesting all changes
  adding changesets
  adding manifests
  adding file changes
  added 273 changesets with 17565 changes to 15175 files
  10622 files updated, 0 files merged, 0 files removed, 0 files unresolved

..
	This part of the process is the script downloading the Network Simulation
	Cradle for you. Note that NSC is not supported on OSX or Cygwin and works 
	best with gcc-3.4 or gcc-4.2 or greater series.

Neste momento, o `script` de download baixa o `Network Simulation Cradle` - NSC. Note que o NSC não é suportado no OSX ou Cygwin e trabalha melhor com o gcc-3.4, gcc-4.2 ou superiores.

..
	After the download.py script completes, you should have several new directories
	under ``~/repos/ns-3-allinone``:

Depois que o `script` ``download.py`` tiver completado sua tarefa, veremos vários diretórios novos dentro de ``~/repos/ns-3-allinone``:

::

  build.py*     constants.pyc  download.py*  nsc/        README      util.pyc
  constants.py  dist.py*       ns-3-dev/     pybindgen/  util.py

..
	Go ahead and change into ``ns-3-dev`` under your ``~/repos/ns-3-allinone`` 
	directory.  You should see something like the following there:

Por fim, no diretório ``ns-3-dev`` que está dentro do diretório ``~/repos/ns-3-allinone`` deve existir, depois dos passos anteriores, o seguinte conteúdo:

::

  AUTHORS       doc       ns3            scratch   testpy.supp  VERSION   waf-tools
  bindings      examples  README         src       utils        waf*      wscript
  CHANGES.html  LICENSE   RELEASE_NOTES  test.py*  utils.py     waf.bat*  wutils.py

..
	You are now ready to build the |ns3| distribution.

Agora está tudo pronto para a construção da distribuição do |ns3|.

.. Downloading ns-3 Using a Tarball

Obtendo o ns-3 compactado (`Tarball`)
+++++++++++++++++++++++++++++++++++++++

..
	The process for downloading |ns3| via tarball is simpler than the
	Mercurial process since all of the pieces are pre-packaged for you.  You just
	have to pick a release, download it and decompress it.

O processo de download do |ns3| compactado é mais simples do que o processo usando o Mercurial, porque tudo que precisamos já vem empacotado. Basta escolher a versão, baixá-la e extraí-la.

..
	As mentioned above, one practice is to create a directory called ``repos``
	in one's home directory under which one can keep local Mercurial repositories.
	One could also keep a ``tarballs`` directory.  *Hint:  the tutorial
	will assume you downloaded into a ``repos`` directory, so remember the
	placekeeper.``*  If you adopt the ``tarballs`` directory approach, you can 
	get a copy of a release by typing the following into your Linux shell 
	(substitute the appropriate version numbers, of course):

Como mencionado anteriormente, uma boa prática é criar um diretório chamado ``repos`` no diretório `home` para manter a cópia local dos repositórios do Mercurial. Da mesma forma, pode-se manter também um diretório chamado ``tarballs`` para manter as versões obtidas via arquivo compactado. *Dica: o tutorial irá assumir que o download foi feito dentro do diretório ``repos``*. Se a opção for pelo método do arquivo compactado, pode-se obter a cópia de uma versão digitando os seguintes comandos no `shell` Linux (obviamente, substitua os números de versões do comando para o valor apropriado):

::

  cd
  mkdir tarballs
  cd tarballs
  wget http://www.nsnam.org/releases/ns-allinone-3.10.tar.bz2
  tar xjf ns-allinone-3.10.tar.bz2

.. 
	If you change into the directory ``ns-allinone-3.10`` you should see a
	number of files:

Dentro do diretório ``ns-allinone-3.10`` extraído, deverá haver algo como:

::

  build.py      ns-3.10/      pybindgen-0.15.0/    util.py
  constants.py  nsc-0.5.2/    README  

..
	You are now ready to build the |ns3| distribution.

Agora está tudo pronto para a construção da distribuição do |ns3|.

.. Building ns-3

Construindo o ns-3
******************

.. Building with build.py

Construindo com o ``build.py``
++++++++++++++++++++++++++++++
..
	The first time you build the |ns3| project you should build using the
	``allinone`` environment.  This will get the project configured for you
	in the most commonly useful way.

A primeira construção do |ns3| deve ser feita usando o ambiente ``allinone``. Isto fará com que o projeto seja configurado da maneira mais funcional.

..
	Change into the directory you created in the download section above.  If you
	downloaded using Mercurial you should have a directory called 
	``ns-3-allinone`` under your ``~/repos`` directory.  If you downloaded
	using a tarball you should have a directory called something like 
	``ns-allinone-3.10`` under your ``~/tarballs`` directory.  Take a deep
	breath and type the following:

Entre no diretório criado na seção "Obtendo o ns-3". Se o Mercurial foi utilizado, então haverá um diretório chamado ``ns-3-allinone`` localizado dentro de ``~/repos``. Se foi utilizado o arquivo compactado, haverá um diretório chamado ``ns-allinone-3.10`` dentro do diretório ``~/tarballs``. Lembre-se de adequar os nomes conforme os arquivos obtidos e diretórios criados. Agora, digite o seguinte comando:

::

  ./build.py --enable-examples --enable-tests

..
	Because we are working with examples and tests in this tutorial, and
	because they are not built by default in |ns3|, the arguments for
	build.py tells it to build them for us.  In the future you can build
	|ns3| without examples and tests if you wish.

Foram utilizadas as opções ``--enable-examples`` e ``--enable-tests`` pois o tutorial irá trabalhar com exemplos e testes, e, por padrão, eles não são construídos. Futuramente, o leitor poderá construir sem estas opções.

..
	You will see lots of typical compiler output messages displayed as the build
	script builds the various pieces you downloaded.  Eventually you should see the
	following magic words:

Serão exibidas muitas saídas típicas de um compilador conforme o código é construído. Finalmente, no final do processo, deverá aparecer uma saída como esta:

::

  Waf: Leaving directory `/home/craigdo/repos/ns-3-allinone/ns-3-dev/build'
  'build' finished successfully (2m30.586s)
  
  Modules built: 
  aodv                      applications              bridge
  click                     config-store              core
  csma                      csma-layout               dsdv
  emu                       energy                    flow-monitor
  internet                  lte                       mesh
  mobility                  mpi                       netanim
  network                   nix-vector-routing        ns3tcp
  ns3wifi                   olsr                      openflow
  point-to-point            point-to-point-layout     propagation
  spectrum                  stats                     tap-bridge
  template                  test                      tools
  topology-read             uan                       virtual-net-device
  visualizer                wifi                      wimax

..
	Once the project has built you can say goodbye to your old friends, the 
	``ns-3-allinone`` scripts.  You got what you needed from them and will now 
	interact directly with Waf and we do it in the ``ns-3-dev`` directory,
	not in the ``ns-3-allinone`` directory.  Go ahead and change into the 
	``ns-3-dev`` directory (or the directory for the appropriate release you
	downloaded.

Uma vez que o projeto foi construído, pode-se deixar de lado os `scripts` ``ns-3-allinone``. O leitor já obteve o que precisava e agora irá interagir diretamente com o Waf no diretório ``ns-3-dev``. Mude para o diretório ``ns-3-dev`` (ou para o diretório apropriado de sua versão).

::

  cd ns-3-dev

.. Building with Waf

Construindo com o Waf
+++++++++++++++++++++

..
	We use Waf to configure and build the |ns3| project.  It's not 
	strictly required at this point, but it will be valuable to take a slight
	detour and look at how to make changes to the configuration of the project.
	Probably the most useful configuration change you can make will be to 
	build the optimized version of the code.  By default you have configured
	your project to build the debug version.  Let's tell the project to 
	make an optimized build.  To explain to Waf that it should do optimized
	builds that include the examples and tests, you will need to execute the 
	following command,

O Waf é utilizado para configurar e construir o projeto do |ns3|. Não é estritamente necessário neste ponto, mas será valioso quando se forem necessárias alterações nas configurações do projeto. Provavelmente a mudança mais útil que será feita futuramente é a construção de uma versão do código otimizado. Por padrão, o projeto é construído com a opção de depuração (`debug`), para verificação de erros. Então, para construir um projeto otimizado, deve-se executar o seguinte comando (ainda com suporte a testes e exemplos):

::

  ./waf -d optimized --enable-examples --enable-tests configure

..
	This runs Waf out of the local directory (which is provided as a convenience
	for you).  As the build system checks for various dependencies you should see
	output that looks similar to the following,

Isto executa o Waf fora do diretório local (o que é bem conveniente). Como o sistema em construção verifica várias dependências, deverá aparecer uma saída similar com a que segue:

::

  Checking for program g++                 : ok /usr/bin/g++
  Checking for program cpp                 : ok /usr/bin/cpp
  Checking for program ar                  : ok /usr/bin/ar
  Checking for program ranlib              : ok /usr/bin/ranlib
  Checking for g++                         : ok
  Checking for program pkg-config          : ok /usr/bin/pkg-config
  Checking for -Wno-error=deprecated-declarations support : yes
  Checking for -Wl,--soname=foo support                   : yes
  Checking for header stdlib.h                            : ok
  Checking for header signal.h                            : ok
  Checking for header pthread.h                           : ok
  Checking for high precision time implementation         : 128-bit integer
  Checking for header stdint.h                            : ok
  Checking for header inttypes.h                          : ok
  Checking for header sys/inttypes.h                      : not found
  Checking for library rt                                 : ok
  Checking for header netpacket/packet.h                  : ok
  Checking for pkg-config flags for GSL                   : ok
  Checking for header linux/if_tun.h                      : ok
  Checking for pkg-config flags for GTK_CONFIG_STORE      : ok
  Checking for pkg-config flags for LIBXML2               : ok
  Checking for library sqlite3                            : ok
  Checking for NSC location                               : ok ../nsc (guessed)
  Checking for library dl                                 : ok
  Checking for NSC supported architecture x86_64          : ok
  Checking for program python                             : ok /usr/bin/python
  Checking for Python version >= 2.3                      : ok 2.5.2
  Checking for library python2.5                          : ok
  Checking for program python2.5-config                   : ok /usr/bin/python2.5-config
  Checking for header Python.h                            : ok
  Checking for -fvisibility=hidden support                : yes
  Checking for pybindgen location                         : ok ../pybindgen (guessed)
  Checking for Python module pybindgen                    : ok
  Checking for pybindgen version                          : ok 0.10.0.640
  Checking for Python module pygccxml                     : ok
  Checking for pygccxml version                           : ok 0.9.5
  Checking for program gccxml                             : ok /usr/local/bin/gccxml
  Checking for gccxml version                             : ok 0.9.0
  Checking for program sudo                               : ok /usr/bin/sudo
  Checking for program hg                                 : ok /usr/bin/hg
  Checking for program valgrind                           : ok /usr/bin/valgrind
  ---- Summary of optional NS-3 features:
  Threading Primitives          : enabled
  Real Time Simulator           : enabled
  Emulated Net Device           : enabled
  GNU Scientific Library (GSL)  : enabled
  Tap Bridge                    : enabled
  GtkConfigStore                : enabled
  XmlIo                         : enabled
  SQlite stats data output      : enabled
  Network Simulation Cradle     : enabled
  Python Bindings               : enabled
  Python API Scanning Support   : enabled
  Use sudo to set suid bit      : not enabled (option --enable-sudo not selected)
  Build tests                   : enabled
  Build examples                : enabled
  Static build                  : not enabled (option --enable-static not selected)
  'configure' finished successfully (2.870s)

..
	Note the last part of the above output.  Some ns-3 options are not enabled by
	default or require support from the underlying system to work properly.
	For instance, to enable XmlTo, the library libxml-2.0 must be found on the
	system.  If this library were not found, the corresponding |ns3| feature 
	would not be enabled and a message would be displayed.  Note further that there is 
	a feature to use the program ``sudo`` to set the suid bit of certain programs.
	This is not enabled by default and so this feature is reported as "not enabled."

Repare a última parte da saída. Algumas opções do ns-3 não estão habilitadas por padrão ou necessitam de algum suporte do sistema para funcionar corretamente. Por exemplo, para habilitar XmlTo, a biblioteca libxml-2.0 deve estar presente no sistema. Se a biblioteca não estiver instalada esta funcionalidade não é habilitada no |ns3| e uma mensagem é apresentada. Note também que existe uma funcionalidade que utiliza o Sudo para configurar o `suid` de certos programas. Isto não está habilitado por padrão, então esta funcionalidade é reportada como não habilitada (``not enabled``). 

.. 
	Now go ahead and switch back to the debug build that includes the examples and tests.

Vamos configurar uma construção do |ns3| com suporte a depuração, bem como, vamos incluir exemplos e testes. Para isto devemos executar:

::

  ./waf -d debug --enable-examples --enable-tests configure

..
	The build system is now configured and you can build the debug versions of 
	the |ns3| programs by simply typing,

Pronto o sistema está configurado, agora podemos construir nossa versão digitando:

::

  ./waf

..
	Some waf commands are meaningful during the build phase and some commands are valid
	in the configuration phase.  For example, if you wanted to use the emulation 
	features of |ns3|, you might want to enable setting the suid bit using
	sudo as described above.  This turns out to be a configuration-time command, and so 
	you could reconfigure using the following command that also includes the examples and tests

Alguns comandos do Waf são válidos apenas na fase de construção e outros são válidos na fase de configuração do sistema. Por exemplo, se o leitor espera usar características de emulação do |ns3|, deve habilitar o `suid` usando o Sudo como descrito anteriormente, isto na fase de configuração. O comando utilizado, incluindo exemplos e testes, será:

::

  ./waf -d debug --enable-sudo --enable-examples --enable-tests configure

..
	If you do this, waf will have run sudo to change the socket creator programs of the
	emulation code to run as root.  There are many other configure- and build-time options
	available in waf.  To explore these options, type:

Com esta configuração, o Waf executará o Sudo para alterar programas que criam soquetes para executar o código de emulação como `root`. Existem várias outras opções de configuração e construção disponíveis no Waf. Para explorar estas opções, digite:

::

  ./waf --help

..
	We'll use some of the testing-related commands in the next section.

Alguns comandos de teste serão utilizados na próxima seção.

..
	Okay, sorry, I made you build the |ns3| part of the system twice,
	but now you know how to change the configuration and build optimized code.

Como pôde ser notado, a construção do |ns3| foi feita duas vezes. Isto para que o leitor saiba como trocar a configuração para construir código otimizado no futuro.

.. Testing ns-3

Testando o ns-3
***************

..
	You can run the unit tests of the |ns3| distribution by running the 
	"./test.py -c core" script,

Para executar os testes de unidade do |ns3|, basta chamar o arquivo ``./test.py`` da seguinte forma:

::

  ./test.py -c core

..
	These tests are run in parallel by waf. You should eventually
	see a report saying that,

Estes testes são executados em paralelo pelo Waf. No final, uma mensagem como a que segue deve aparecer.

::

  47 of 47 tests passed (47 passed, 0 failed, 0 crashed, 0 valgrind errors)

..
	This is the important message.

Esta é uma mensagem importante.

..	
	You will also see output from the test runner and the output will actually look something like,

Também haverá saídas da execução do teste e estas geralmente são algo como:

::

  Waf: Entering directory `/home/craigdo/repos/ns-3-allinone/ns-3-dev/build'
  Waf: Leaving directory `/home/craigdo/repos/ns-3-allinone/ns-3-dev/build'
  'build' finished successfully (1.799s)
  
  Modules built: 
  aodv                      applications              bridge
  click                     config-store              core
  csma                      csma-layout               dsdv
  emu                       energy                    flow-monitor
  internet                  lte                       mesh
  mobility                  mpi                       netanim
  network                   nix-vector-routing        ns3tcp
  ns3wifi                   olsr                      openflow
  point-to-point            point-to-point-layout     propagation
  spectrum                  stats                     tap-bridge
  template                  test                      tools
  topology-read             uan                       virtual-net-device
  visualizer                wifi                      wimax

  PASS: TestSuite ns3-wifi-interference
  PASS: TestSuite histogram
  PASS: TestSuite sample
  PASS: TestSuite ipv4-address-helper
  PASS: TestSuite devices-wifi
  PASS: TestSuite propagation-loss-model

  ...

  PASS: TestSuite attributes
  PASS: TestSuite config
  PASS: TestSuite global-value
  PASS: TestSuite command-line
  PASS: TestSuite basic-random-number
  PASS: TestSuite object
  PASS: TestSuite random-number-generators
  95 of 95 tests passed (95 passed, 0 failed, 0 crashed, 0 valgrind errors)

..
	This command is typically run by ``users`` to quickly verify that an 
	|ns3| distribution has built correctly.  

Este comando é normalmente executado pelos usuários para verificar se o |ns3| foi construído corretamente. 

.. Running a Script

Executando um código (`Script`)
*******************************

..
	We typically run scripts under the control of Waf.  This allows the build 
	system to ensure that the shared library paths are set correctly and that
	the libraries are available at run time.  To run a program, simply use the
	``--run`` option in Waf.  Let's run the |ns3| equivalent of the
	ubiquitous hello world program by typing the following:

Os códigos normalmente são executados sob o controle do Waf. Isto assegura que os caminhos das bibliotecas compartilhadas estejam corretas e que estarão disponíveis em tempo de execução. Para executar um programa, basta usar a opção ``--run`` no Waf. Para executar um equivalente ao "Olá mundo" (`Hello world`) no |ns3|, utilizamos o comando:

::

  ./waf --run hello-simulator

..
	Waf first checks to make sure that the program is built correctly and 
	executes a build if required.  Waf then executes the program, which 
	produces the following output.

O Waf primeiro verifica se o programa foi construído corretamente e se necessário, o constrói. Então executa o programa, que fornece a seguinte saída:

::

  Hello Simulator

.. 
	*Congratulations.  You are now an ns-3 user.*

*Parabéns. Você agora é um usuário ns-3*

..
	*What do I do if I don't see the output?*

*O que fazer se o comando não gerar uma saída?*

..
	If you see ``waf`` messages indicating that the build was 
	completed successfully, but do not see the "Hello Simulator" output, 
	chances are that you have switched your build mode to "optimized" in 
	the "Building with Waf" section, but have missed the change back to 
	"debug" mode.  All of the console output used in this tutorial uses a 
	special |ns3| logging component that is useful for printing 
	user messages to the console.  Output from this component is 
	automatically disabled when you compile optimized code -- it is 
	"optimized out."  If you don't see the "Hello Simulator" output,
	type the following,

Se a mensagem ``Hello Simulator`` não aparece, mas o Waf gera saídas indicando que a construção do sistema foi executada com sucesso, é possível que o leitor tenha trocado o sistema para o modo otimizado na seção `Construindo com o Waf`, e tenha esquecido de voltar ao modo de depuração. Todas as saídas utilizadas neste tutorial são feitas com um componente especial de registro (`logging`) do |ns3| que é muito útil para mostrar mensagens na tela. As saídas deste componente são automaticamente desabilitadas quando o código é contruído na forma otimizada. Para produzir as saídas, digite o seguinte comando,

::

  ./waf -d debug --enable-examples --enable-tests configure

..
	to tell ``waf`` to build the debug versions of the |ns3| 
	programs that includes the examples and tests.  You must still build 
	the actual debug version of the code by typing,

para dizer ao Waf para construir o |ns3| com a versão de depuração e incluir exemplos e testes. Ainda é necessário digitar o seguinte comando para a construção:

::

  ./waf

..
	Now, if you run the ``hello-simulator`` program, you should see the 
	expected output.

Agora, ao executar o programa ``hello-simulator`` devemos ter a seguinte a saída.

..
	If you want to run programs under another tool such as gdb or valgrind,
	see this `wiki entry
	<http://www.nsnam.org/wiki/User_FAQ#How_to_run_NS-3_programs_under_another_tool>`_.

Se o leitor for executar seus programas sob outras ferramentas, tais como Gdb ou Valgrind, é recomendável que leia a seguinte `entrada no Wiki <http://www.nsnam.org/wiki/User_FAQ#How_to_run_NS-3_programs_under_another_tool>`_.

