.. include:: replace.txt

..
	========================================================================================
	Translated for portuguese by the students of the inter-institutional doctorate program of IME-USP/UTFPR-CM.
	
	Traduzido para o português pelos alunos do programa de doutorado inter institucional do Instituto de Matemática e Estatística da Universidade de São Paulo --- IME-USP em parceria com a Universidade Tecnológica Federal do Paraná - Campus Campo Mourão --- UTFPR-CM:
	
	* Frank Helbert (frank@ime.usp.br);
	* Luiz Arthur Feitosa dos Santos (luizsan@ime.usp.br);
	* Rodrigo Campiolo (campiolo@ime.usp.br).
	========================================================================================

..
	Tracing

Rastreamento
------------

..
	Background

Introdução
**********

..
	As mentioned in the Using the Tracing System section, the whole point of running
	an |ns3| simulation is to generate output for study.  You have two basic 
	strategies to work with in |ns3|: using generic pre-defined bulk output 
	mechanisms and parsing their content to extract interesting information; or 
	somehow developing an output mechanism that conveys exactly (and perhaps only) 
	the information wanted.

Como abordado na seção Usando o Sistema de Rastreamento, o objetivo principal de uma
simulação no |ns3| é a geração de saída para estudo. Há duas estratégias básicas: 
usar mecanismos predefinidos de saída e processar o conteúdo para extrair informações
relevantes; ou desenvolver mecanismos de saída que resultam somente ou exatamente na
informação pretendida.

..
	Using pre-defined bulk output mechanisms has the advantage of not requiring any
	changes to |ns3|, but it does require programming.  Often, pcap or NS_LOG
	output messages are gathered during simulation runs and separately run through 
	scripts that use grep, sed or awk to parse the messages and reduce and transform
	the data to a manageable form.  Programs must be written to do the 
	transformation, so this does not come for free.  Of course, if the information
	of interest in does not exist in any of the pre-defined output mechanisms,
	this approach fails.

Usar mecanismos predefinidos de saída possui a vantagem de não necessitar modificações 
no |ns3|, mas requer programação. Geralmente, as mensagens de saída do pcap ou ``NS_LOG``
são coletadas durante a execução da simulação e processadas separadamente por códigos (`scripts`) que usam `grep`, `sed` ou `awk` para reduzir e transformar os dados para uma forma mais simples de gerenciar. Há o custo do desenvolvimento de programas para realizar as transformações e em algumas situações a informação de interesse pode não estar contida em nenhuma das saídas, logo, a abordagem falha.

..
	If you need to add some tidbit of information to the pre-defined bulk mechanisms,
	this can certainly be done; and if you use one of the |ns3| mechanisms, 
	you may get your code added as a contribution.

Se precisarmos adicionar o mínimo de informação para os mecanismos predefinidos de saída, isto certamente pode ser feito e se usarmos os mecanismos do |ns3|, podemos
ter nosso código adicionado como uma contribuição.

..
	|ns3| provides another mechanism, called Tracing, that avoids some of the 
	problems inherent in the bulk output mechanisms.  It has several important 
	advantages.  First, you can reduce the amount of data you have to manage by only
	tracing the events of interest to you (for large simulations, dumping everything
	to disk for post-processing can create I/O bottlenecks).  Second, if you use this
	method, you can control the format of the output directly so you avoid the 
	postprocessing step with sed or awk script.  If you desire, your output can be 
	formatted directly into a form acceptable by gnuplot, for example.  You can add 
	hooks in the core which can then be accessed by other users, but which will 
	produce no information unless explicitly asked to do so.  For these reasons, we 
	believe that the |ns3| tracing system is the best way to get information 
	out of a simulation and is also therefore one of the most important mechanisms
	to understand in |ns3|.

O |ns3| fornece outro mecanismo, chamado Rastreamento (*Tracing*), que evita alguns dos
problemas associados com os mecanismos de saída predefinidos. Há várias vantagens. Primeiro, redução da quantidade de dados para gerenciar (em simulações grandes, armazenar toda saída no disco pode gerar gargalos de Entrada/Saída). Segundo, o formato da saída pode ser controlado diretamente evitando o pós-processamento com códigos `sed` ou `awk`. Se desejar,
a saída pode ser processada diretamente para um formato reconhecido pelo `gnuplot`, por exemplo. Podemos adicionar ganchos ("`hooks`") no núcleo, os quais podem ser acessados por outros usuários, mas que não produzirão nenhuma informação exceto que sejam explicitamente solicitados a produzir. Por essas razões, acreditamos que o sistema de rastreamento do |ns3| é a melhor forma de obter informações fora da simulação, portanto é um dos mais importantes mecanismos para ser compreendido no |ns3|.

..
	Blunt Instruments

Métodos Simples
+++++++++++++++

..
	There are many ways to get information out of a program.  The most 
	straightforward way is to just directly print the information to the standard 
	output, as in,

Há várias formas de obter informação após a finalização de um programa. A mais direta
é imprimir a informação na saída padrão, como no exemplo,

::

  #include <iostream>
  ...
  void
  SomeFunction (void)
  {
    uint32_t x = SOME_INTERESTING_VALUE;
    ...
    std::cout << "The value of x is " << x << std::endl;
    ...
  } 

..
	Nobody is going to prevent you from going deep into the core of |ns3| and
	adding print statements.  This is insanely easy to do and, after all, you have 
	complete control of your own |ns3| branch.  This will probably not turn 
	out to be very satisfactory in the long term, though.

Ninguém impedirá que editemos o núcleo do |ns3| e adicionemos códigos de impressão. Isto é simples de fazer, além disso temos controle e acesso total ao código fonte do |ns3|. Entretanto, pensando no futuro, isto não é muito interessante.

..
	As the number of print statements increases in your programs, the task of 
	dealing with the large number of outputs will become more and more complicated.  
	Eventually, you may feel the need to control what information is being printed 
	in some way; perhaps by turning on and off certain categories of prints, or 
	increasing or decreasing the amount of information you want.  If you continue 
	down this path you may discover that you have re-implemented the ``NS_LOG``
	mechanism.  In order to avoid that, one of the first things you might consider
	is using ``NS_LOG`` itself.

Conforme aumentarmos o número de comandos de impressão em nossos programas, ficará mais difícil tratar a grande quantidade de saídas. Eventualmente, precisaremos controlar de alguma maneira qual a informação será impressa; talvez habilitando ou não determinadas categorias de saídas, ou aumentando ou diminuindo a quantidade de informação desejada. Se continuarmos com esse processo, descobriremos depois de um tempo que, reimplementamos o mecanismo ``NS_LOG``. Para evitar isso, utilize o próprio ``NS_LOG``.

..
	We mentioned above that one way to get information out of |ns3| is to 
	parse existing NS_LOG output for interesting information.  If you discover that 
	some tidbit of information you need is not present in existing log output, you 
	could edit the core of |ns3| and simply add your interesting information
	to the output stream.  Now, this is certainly better than adding your own
	print statements since it follows |ns3| coding conventions and could 
	potentially be useful to other people as a patch to the existing core.

Como abordado anteriormente, uma maneira de obter informação de saída do |ns3| é 
processar a saída do ``NS_LOG``, filtrando as informações relevantes. Se a informação
não está presente nos registros existentes, pode-se editar o núcleo do |ns3| e 
adicionar ao fluxo de saída a informação desejada. Claro, isto é muito melhor
que adicionar comandos de impressão, desde que seguindo as convenções de codificação
do |ns3|, além do que isto poderia ser potencialmente útil a outras pessoas.

..
	Let's pick a random example.  If you wanted to add more logging to the 
	|ns3| TCP socket (``tcp-socket-base.cc``) you could just add a new 
	message down in the implementation.  Notice that in TcpSocketBase::ReceivedAck()
	there is no log message for the no ack case.  You could simply add one, 
	changing the code from:

Vamos analisar um exemplo, adicionando mais informações de registro ao `socket` TCP do arquivo ``tcp-socket-base.cc``, para isto vamos acrescentando uma nova mensagem de registro na implementação. Observe que em ``TcpSocketBase::ReceivedAck()`` não existem mensagem de registro para casos sem o ACK, então vamos adicionar uma da seguinte forma:

::

  /** Processa o mais recente ACK recebido */
  void
  TcpSocketBase::ReceivedAck (Ptr<Packet> packet, const TcpHeader& tcpHeader)
  {
    NS_LOG_FUNCTION (this << tcpHeader);

    // ACK Recebido. Compara o número ACK com o mais alto seqno não confirmado
    if (0 == (tcpHeader.GetFlags () & TcpHeader::ACK))
      { // Ignora se não há flag ACK 
      }
    ...

.. 
	to add a new ``NS_LOG_LOGIC`` in the appropriate statement:

para adicionar um novo ``NS_LOG_LOGIC`` na sentença apropriada:

::

  /** Processa o mais recente ACK recebido */
  void
  TcpSocketBase::ReceivedAck (Ptr<Packet> packet, const TcpHeader& tcpHeader)
  {
    NS_LOG_FUNCTION (this << tcpHeader);

    // ACK Recebido. Compara o número ACK com o mais alto seqno não confirmado
    if (0 == (tcpHeader.GetFlags () & TcpHeader::ACK))
      { // Ignora se não há flag ACK 
        NS_LOG_LOGIC ("TcpSocketBase " << this << " sem flag ACK");
      }
    ...

..
	This may seem fairly simple and satisfying at first glance, but something to
	consider is that you will be writing code to add the ``NS_LOG`` statement 
	and you will also have to write code (as in grep, sed or awk scripts) to parse
	the log output in order to isolate your information.  This is because even 
	though you have some control over what is output by the logging system, you 
	only have control down to the log component level.  

Isto pode parecer simples e satisfatório a primeira vista, mas lembre-se que nós escreveremos
código para adicionar ao ``NS_LOG`` e para processar a saída com a finalidade de isolar
a informação de interesse. Isto porque o controle é limitado ao nível do componente de registro.

..
	If you are adding code to an existing module, you will also have to live with the
	output that every other developer has found interesting.  You may find that in 
	order to get the small amount of information you need, you may have to wade 
	through huge amounts of extraneous messages that are of no interest to you.  You
	may be forced to save huge log files to disk and process them down to a few lines
	whenever you want to do anything.

Se cada desenvolvedor adicionar códigos de saída para um módulo existente, logo conviveremos com a saída que outro desenvolvedor achou interessante. É descobriremos que para obter uma pequena quantidade de informação, precisaremos produzir uma volumosa quantidade de mensagens sem nenhuma relevância (devido aos comandos de saída de vários desenvolvedores). Assim seremos forçados a gerar arquivos de registros gigantescos no disco e processá-los para obter poucas linhas de nosso interesse.

..
	Since there are no guarantees in |ns3| about the stability of ``NS_LOG``
	output, you may also discover that pieces of log output on which you depend 
	disappear or change between releases.  If you depend on the structure of the 
	output, you may find other messages being added or deleted which may affect your
	parsing code.

Como não há nenhuma garantia no |ns3| sobre a estabilidade da saída do ``NS_LOG``, podemos descobrir que partes do registro de saída, que dependíamos, desapareceram ou mudaram entre versões. Se dependermos da estrutura da saída, podemos encontrar outras mensagens sendo adicionadas ou removidas que podem afetar seu código de processamento.

..
	For these reasons, we consider prints to ``std::cout`` and NS_LOG messages 
	to be quick and dirty ways to get more information out of |ns3|.

Por estas razões, devemos considerar o uso do ``std::cout`` e as mensagens ``NS_LOG`` como formas rápidas e porém sujas de obter informação da saída no |ns3|.

..
	It is desirable to have a stable facility using stable APIs that allow one to 
	reach into the core system and only get the information required.  It is
	desirable to be able to do this without having to change and recompile the
	core system.  Even better would be a system that notified the user when an item
	of interest changed or an interesting event happened so the user doesn't have 
	to actively poke around in the system looking for things.

Na grande maioria dos casos desejamos ter um mecanismo estável, usando APIs que permitam acessar o núcleo do sistema e obter somente informações interessantes. Isto deve ser possível sem que exista a necessidade de alterar e recompilar o núcleo do sistema. Melhor ainda seria se um sistema notificasse o usuário quando um item de interesse fora modificado ou um evento de interesse aconteceu, pois o usuário não teria que constantemente vasculhar o sistema procurando por coisas.

..
	The |ns3| tracing system is designed to work along those lines and is 
	well-integrated with the Attribute and Config subsystems allowing for relatively
	simple use scenarios.

O sistema de rastreamento do |ns3| é projetado para trabalhar seguindo essas premissas e é 
integrado com os subsistemas de Atributos (*Attribute*) e Configuração (*Config*) permitindo cenários de uso simples.

.. 
	Overview

Visão Geral
***********

..
	The ns-3 tracing system is built on the concepts of independent tracing sources
	and tracing sinks; along with a uniform mechanism for connecting sources to sinks.

O sistema de rastreamento do |ns3| é baseado no conceito independente origem do rastreamento e destino do rastreamento. O |ns3| utiliza um mecanismo uniforme para conectar origens a destinos.

..
	Trace sources are entities that can signal events that happen in a simulation and 
	provide access to interesting underlying data.  For example, a trace source could
	indicate when a packet is received by a net device and provide access to the 
	packet contents for interested trace sinks.  A trace source might also indicate 
	when an interesting state change happens in a model.  For example, the congestion
	window of a TCP model is a prime candidate for a trace source.

As origens do rastreamento (*trace source*) são entidades que podem assinalar eventos que ocorrem na simulação e fornecem acesso a dados de baixo nível. Por exemplo, uma origem do rastreamento poderia indicar quando um pacote é recebido por um dispositivo de rede e prove acesso ao conteúdo do pacote aos interessados no destino do rastreamento. Uma origem do rastreamento pode também indicar quando uma mudança de estado ocorre em um modelo. Por exemplo, a janela de congestionamento do modelo TCP é um forte candidato para uma origem do rastreamento.

..
	Trace sources are not useful by themselves; they must be connected to other pieces
	of code that actually do something useful with the information provided by the source.
	The entities that consume trace information are called trace sinks.  Trace sources 
	are generators of events and trace sinks are consumers.  This explicit division 
	allows for large numbers of trace sources to be scattered around the system in 
	places which model authors believe might be useful.  

A origem do rastreamento não são úteis sozinhas; elas devem ser conectadas a outras partes de código que fazem algo útil com a informação provida pela origem. As entidades que consomem a informação de rastreamento são chamadas de destino do rastreamento (*trace sinks*). As origens de rastreamento são geradores de eventos e destinos de rastreamento são consumidores. Esta divisão explícita permite que inúmeras origens de rastreamento estejam dispersas no sistema em locais que os autores do modelo acreditam ser úteis.

..
	There can be zero or more consumers of trace events generated by a trace source.  
	One can think of a trace source as a kind of point-to-multipoint information link.  
	Your code looking for trace events from a particular piece of core code could 
	happily coexist with other code doing something entirely different from the same
	information.

Pode haver zero ou mais consumidores de eventos de rastreamento gerados por uma origem do rastreamento. Podemos pensar em uma origem do rastreamento como um tipo de ligação de informação ponto-para-multiponto. Seu código buscaria por eventos de rastreamento de uma parte específica do código do núcleo e poderia coexistir com outro código que faz algo inteiramente diferente com a mesma informação.

..
	Unless a user connects a trace sink to one of these sources, nothing is output.  By
	using the tracing system, both you and other people at the same trace source are 
	getting exactly what they want and only what they want out of the system.  Neither
	of you are impacting any other user by changing what information is output by the 
	system.  If you happen to add a trace source, your work as a good open-source 
	citizen may allow other users to provide new utilities that are perhaps very useful
	overall, without making any changes to the |ns3| core.  

Ao menos que um usuário conecte um destino do rastreamento a uma destas origens, nenhuma saída é produzida. Usando o sistema de rastreamento, todos conectados em uma mesma origem do rastreamento estão obtendo a informação que desejam do sistema. Um usuário não afeta os outros alterando a informação provida pela origem. Se acontecer de adicionarmos uma origem do rastreamento, seu trabalho como um bom cidadão utilizador de código livre pode permitir que outros usuários forneçam novas utilidades para todos, sem fazer qualquer modificação no núcleo do |ns3|.
	
.. 
	A Simple Low-Level Example

Um Exemplo Simples de Baixo Nível
+++++++++++++++++++++++++++++++++

..
	Let's take a few minutes and walk through a simple tracing example.  We are going
	to need a little background on Callbacks to understand what is happening in the
	example, so we have to take a small detour right away.

Vamos gastar alguns minutos para entender um exemplo de rastreamento simples. Primeiramente
precisamos compreender o conceito de *callbacks* para entender o que está acontecendo
no exemplo.

*Callbacks*
~~~~~~~~~~~

..
	The goal of the Callback system in |ns3| is to allow one piece of code to 
	call a function (or method in C++) without any specific inter-module dependency.
	This ultimately means you need some kind of indirection -- you treat the address
	of the called function as a variable.  This variable is called a pointer-to-function
	variable.  The relationship between function and pointer-to-function pointer is 
	really no different that that of object and pointer-to-object.

O objetivo do sistema de *Callback*, no |ns3|, é permitir a uma parte do código invocar
uma função (ou método em C++) sem qualquer dependência entre módulos. Isto é utilizado para prover algum tipo de indireção -- desta forma tratamos o endereço da chamada de função como uma variável. Esta variável é denominada variável de ponteiro-para-função. O relacionamento entre função e ponteiro-para-função não é tão diferente que de um objeto e ponteiro-para-objeto.

..
	In C the canonical example of a pointer-to-function is a 
	pointer-to-function-returning-integer (PFI).  For a PFI taking one int parameter,
	this could be declared like,

Em C, o exemplo clássico de um ponteiro-para-função é um ponteiro-para-função-retornando-inteiro (PFI). Para um PFI ter um parâmetro inteiro, poderia ser declarado como,

::

  int (*pfi)(int arg) = 0;

..
	What you get from this is a variable named simply "pfi" that is initialized
	to the value 0.  If you want to initialize this pointer to something meaningful,
	you have to have a function with a matching signature.  In this case, you could
	provide a function that looks like,

O código descreve uma variável nomeada como "pfi" que é inicializada com o valor 0. Se quisermos inicializar este ponteiro com um valor significante, temos que ter uma função com uma assinatura idêntica. Neste caso, poderíamos prover uma função como,

::

  int MyFunction (int arg) {}

..
	If you have this target, you can initialize the variable to point to your
	function:

Dessa forma, podemos inicializar a variável apontando para uma função:

::

  pfi = MyFunction;

..
	You can then call MyFunction indirectly using the more suggestive form of
	the call,

Podemos então chamar ``MyFunction`` indiretamente, usando uma forma mais clara da chamada,

::

  int result = (*pfi) (1234);

..
	This is suggestive since it looks like you are dereferencing the function
	pointer just like you would dereference any pointer.  Typically, however,
	people take advantage of the fact that the compiler knows what is going on
	and will just use a shorter form,

É uma forma mais clara, pois é como se estivéssemos dereferenciando o ponteiro da função como dereferenciamos qualquer outro ponteiro. Tipicamente, todavia, usa-se uma forma mais curta pois o compilador sabe o que está fazendo,

::

  int result = pfi (1234);

..
	This looks like you are calling a function named "pfi," but the compiler is
	smart enough to know to call through the variable ``pfi`` indirectly to
	the function ``MyFunction``.

Esta forma é como se estivessemos chamando uma função nomeada "pfi", mas o compilador reconhece que é uma chamada indireta da função ``MyFunction`` por meio da variável ``pfi``.

..
	Conceptually, this is almost exactly how the tracing system will work.
	Basically, a trace source *is* a callback.  When a trace sink expresses
	interest in receiving trace events, it adds a Callback to a list of Callbacks
	internally held by the trace source.  When an interesting event happens, the 
	trace source invokes its ``operator()`` providing zero or more parameters.
	The ``operator()`` eventually wanders down into the system and does something
	remarkably like the indirect call you just saw.  It provides zero or more 
	parameters (the call to "pfi" above passed one parameter to the target function
	``MyFunction``.

Conceitualmente, é quase exatamente como o sistema de rastreamento funciona. Basicamente, uma origem do rastreamento *é* um *callback*. Quando um destino do rastreamento expressa interesse em receber eventos de rastreamento, ela adiciona a *callback* para a lista de *callbacks*  mantida internamente pela origem do rastreamento. Quando um evento de interesse ocorre, a origem do rastreamento invoca seu ``operator()`` provendo zero ou mais parâmetros. O ``operator()`` eventualmente percorre o sistema e faz uma chamada indireta com zero ou mais parâmetros.
	
..
	The important difference that the tracing system adds is that for each trace
	source there is an internal list of Callbacks.  Instead of just making one 
	indirect call, a trace source may invoke any number of Callbacks.  When a trace
	sink expresses interest in notifications from a trace source, it basically just
	arranges to add its own function to the callback list.

Uma diferença importante é que o sistema de rastreamento adiciona para cada origem do rastreamento uma lista interna de *callbacks*. Ao invés de apenas fazer uma chamada indireta, uma origem do rastreamento pode invocar qualquer número de *callbacks*. Quando um destino do rastreamento expressa interesse em notificações de uma origem, ela adiciona sua própria função para a lista de *callback*.

..
	If you are interested in more details about how this is actually arranged in 
	|ns3|, feel free to peruse the Callback section of the manual.

Estando interessado em mais detalhes sobre como é organizado o sistema de *callback* no |ns3|, leia a seção *Callback* do manual.

.. 
	Example Code

Código de Exemplo
~~~~~~~~~~~~~~~~~

..
	We have provided some code to implement what is really the simplest example
	of tracing that can be assembled.  You can find this code in the tutorial
	directory as ``fourth.cc``.  Let's walk through it.

Analisaremos uma implementação simples de um exemplo de rastreamento. Este código está no diretório do tutorial, no arquivo ``fourth.cc``.

::

  /* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
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
   * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
   */
  
  #include "ns3/object.h"
  #include "ns3/uinteger.h"
  #include "ns3/traced-value.h"
  #include "ns3/trace-source-accessor.h"
  
  #include <iostream>
  
  using namespace ns3;

..
	Most of this code should be quite familiar to you.  As mentioned above, the
	trace system makes heavy use of the Object and Attribute systems, so you will 
	need to include them.  The first two includes above bring in the declarations
	for those systems explicitly.  You could use the core module header, but this
	illustrates how simple this all really is.  

A maior parte deste código deve ser familiar, pois como já abordado, o sistema de rastreamento faz uso constante dos sistemas Objeto (*Object*) e Atributos (*Attribute*), logo é necessário incluí-los. As duas primeiras inclusões (*include*) declaram explicitamente estes dois sistemas. Poderíamos usar o cabeçalho (*header*) do módulo núcleo, este exemplo é simples.

..
	The file, ``traced-value.h`` brings in the required declarations for tracing
	of data that obeys value semantics.  In general, value semantics just means that
	you can pass the object around, not an address.  In order to use value semantics
	at all you have to have an object with an associated copy constructor and 
	assignment operator available.  We extend the requirements to talk about the set
	of operators that are pre-defined for plain-old-data (POD) types.  Operator=, 
	operator++, operator---, operator+, operator==, etc.

O arquivo ``traced-value.h`` é uma declaração obrigatória para rastreamento de dados que usam passagem por valor. Na passagem por valor é passada uma cópia do objeto e não um endereço. Com a finalidade de usar passagem por valor, precisa-se de um objeto com um construtor de cópia associado e um operador de atribuição. O conjunto de operadores predefinidos para tipos de dados primitivos (*plain-old-data*) são ++, ---, +, ==, etc.

..
	What this all really means is that you will be able to trace changes to a C++
	object made using those operators.

Isto significa que somos capazes de rastrear alterações em um objeto C++ usando estes operadores.

..
	Since the tracing system is integrated with Attributes, and Attributes work
	with Objects, there must be an |ns3| ``Object`` for the trace source
	to live in.  The next code snippet declares and defines a simple Object we can
	work with.

Como o sistema de rastreamento é integrado com Atributos e este trabalham com Objetos, deve obrigatoriamente existir um ``Object`` |ns3| para cada origem do rastreamento. O próximo código define e declara um Objeto.

::

  class MyObject : public Object
  {
  public:
    static TypeId GetTypeId (void)
    {
      static TypeId tid = TypeId ("MyObject")
        .SetParent (Object::GetTypeId ())
        .AddConstructor<MyObject> ()
        .AddTraceSource ("MyInteger",
                         "An integer value to trace.",
                         MakeTraceSourceAccessor (&MyObject::m_myInt))
        ;
      return tid;
    }
    
    MyObject () {}
    TracedValue<int32_t> m_myInt;
  };

..
	The two important lines of code, above, with respect to tracing are the 
	``.AddTraceSource`` and the ``TracedValue`` declaration of ``m_myInt``.

As duas linhas mais importantes com relação ao rastreamento são ``.AddTraceSource`` e a declaração ``TracedValue`` do ``m_myInt``.


..
	The ``.AddTraceSource`` provides the "hooks" used for connecting the trace
	source to the outside world through the config system.  The ``TracedValue`` 
	declaration provides the infrastructure that overloads the operators mentioned 
	above and drives the callback process.

O método ``.AddTraceSource`` provê a "ligação" usada para conectar a origem do rastreamento com o mundo externo, por meio do sistema de configuração. A declaração ``TracedValue`` provê a infraestrutura que sobrecarrega os operadores abordados anteriormente e  gerencia o processo de *callback*.

::

  void
  IntTrace (int32_t oldValue, int32_t newValue)
  {
    std::cout << "Traced " << oldValue << " to " << newValue << std::endl;
  }

..
	This is the definition of the trace sink.  It corresponds directly to a callback
	function.  Once it is connected, this function will be called whenever one of the
	overloaded operators of the ``TracedValue`` is executed.

Esta é a definição do destino do rastreamento. Isto corresponde diretamente a função de *callback*. Uma vez que está conectada, esta função será chamada sempre que um dos operadores sobrecarregados de ``TracedValue`` é executado.

..
	We have now seen the trace source and the trace sink.  What remains is code to
	connect the source to the sink.

Nós temos a origem e o destino do rastreamento. O restante é o código para conectar a origem ao destino.

::

  int
  main (int argc, char *argv[])
  {
    Ptr<MyObject> myObject = CreateObject<MyObject> ();
    myObject->TraceConnectWithoutContext ("MyInteger", MakeCallback(&IntTrace));
  
    myObject->m_myInt = 1234;
  }

..
	Here we first create the Object in which the trace source lives.

Criamos primeiro o Objeto no qual está a origem do rastreamento.

..
	The next step, the ``TraceConnectWithoutContext``, forms the connection
	between the trace source and the trace sink.  Notice the ``MakeCallback``
	template function.  This function does the magic required to create the
	underlying |ns3| Callback object and associate it with the function
	``IntTrace``.  TraceConnect makes the association between your provided
	function and the overloaded ``operator()`` in the traced variable referred 
	to by the "MyInteger" Attribute.  After this association is made, the trace
	source will "fire" your provided callback function.

No próximo passo, o ``TraceConnectWithoutContext`` conecta a origem ao destino do rastreamento. Observe que a função ``MakeCallback`` cria o objeto *callback* e associa com a função ``IntTrace``. ``TraceConnectWithoutContext`` faz a associação entre a sua função e o ``operator()``, sobrecarregado a variável rastreada referenciada pelo Atributo ``"MyInteger"``. Depois disso, a origem do rastreamento "disparará" sua função de callback.

..
	The code to make all of this happen is, of course, non-trivial, but the essence
	is that you are arranging for something that looks just like the ``pfi()``
	example above to be called by the trace source.  The declaration of the 
	``TracedValue<int32_t> m_myInt;`` in the Object itself performs the magic 
	needed to provide the overloaded operators (++, ---, etc.) that will use the
	``operator()`` to actually invoke the Callback with the desired parameters.
	The ``.AddTraceSource`` performs the magic to connect the Callback to the 
	Config system, and ``TraceConnectWithoutContext`` performs the magic to
	connect your function to the trace source, which is specified by Attribute
	name.

O código para fazer isto acontecer não é trivial, mas a essência é a mesma que se a origem do rastreamento chamasse a função ``pfi()`` do exemplo anterior. A declaração ``TracedValue<int32_t> m_myInt;`` no Objeto é responsável pela mágica dos operadores sobrecarregados que usarão o ``operator()`` para invocar o *callback*  com os parâmetros desejados. O método ``.AddTraceSource`` conecta o *callback* ao sistema de configuração, e ``TraceConnectWithoutContext`` conecta sua função a fonte de rastreamento, a qual é especificada por um nome 
Atributo.

.. 
	Let's ignore the bit about context for now.

Vamos ignorar um pouco o contexto.

.. 
	Finally, the line,

Finalmente a linha,

::

   myObject->m_myInt = 1234;

..
	should be interpreted as an invocation of ``operator=`` on the member 
	variable ``m_myInt`` with the integer ``1234`` passed as a parameter.

deveria ser interpretada como uma invocação do operador ``=`` na variável membro ``m_myInt`` com o inteiro ``1234`` passado como parâmetro.

..
	It turns out that this operator is defined (by ``TracedValue``) to execute
	a callback that returns void and takes two integer values as parameters --- 
	an old value and a new value for the integer in question.  That is exactly 
	the function signature for the callback function we provided --- ``IntTrace``.

Por sua vez este operador é definido (por ``TracedValue``) para executar um *callback* que retorna ``void`` e possui dois inteiros como parâmetros --- um valor antigo e um novo valor para o inteiro em questão. Isto é exatamente a assinatura da função para a função de *callback* que nós fornecemos --- ``IntTrace``.

..
	To summarize, a trace source is, in essence, a variable that holds a list of
	callbacks.  A trace sink is a function used as the target of a callback.  The
	Attribute and object type information systems are used to provide a way to 
	connect trace sources to trace sinks.  The act of "hitting" a trace source
	is executing an operator on the trace source which fires callbacks.  This 
	results in the trace sink callbacks registering interest in the source being 
	called with the parameters provided by the source.

Para resumir, uma origem do rastreamento é, em essência, uma variável que mantém uma lista de *callbacks*. Um destino do rastreamento é uma função usada como alvo da *callback*. O Atributo e os sistemas de informação de tipo de objeto são usados para fornecer uma maneira de conectar origens e destinos do rastreamento. O ação de "acionar" uma origem do rastreamento é executar um operador na origem, que dispara os *callbacks*. Isto resulta na execução das *callbacks* dos destinos do rastreamento registrados na origem com os parâmetros providos pela origem.

.. 
	If you now build and run this example,

Se compilarmos e executarmos este exemplo,

::

  ./waf --run fourth

..
	you will see the output from the ``IntTrace`` function execute as soon as the
	trace source is hit:

observaremos que a saída da função ``IntTrace`` é processada logo após a execução da
origem do rastreamento:

::

  Traced 0 to 1234

..
	When we executed the code, ``myObject->m_myInt = 1234;``, the trace source 
	fired and automatically provided the before and after values to the trace sink.
	The function ``IntTrace`` then printed this to the standard output.  No 
	problem.

Quando executamos o código,  ``myObject->m_myInt = 1234;`` a origem do rastreamento disparou e automaticamente forneceu os valores anteriores e posteriores para o destino do rastreamento. A função ``IntTrace`` então imprimiu na saída padrão, sem maiores problemas.

.. 
	Using the Config Subsystem to Connect to Trace Sources

Usando o Subsistema de Configuração para Conectar as Origens de Rastreamento
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

..
	The ``TraceConnectWithoutContext`` call shown above in the simple example is
	actually very rarely used in the system.  More typically, the ``Config``
	subsystem is used to allow selecting a trace source in the system using what is
	called a *config path*.  We saw an example of this in the previous section
	where we hooked the "CourseChange" event when we were playing with 
	``third.cc``.

A chamada ``TraceConnectWithoutContext`` apresentada anteriormente é raramente usada no sistema. Geralmente, o subsistema ``Config`` é usado para selecionar uma origem do rastreamento no sistema usando um caminho de configuração (*config path*). Nós estudamos um exemplo onde ligamos o evento "CourseChange", quando estávamos brincando com ``third.cc``.

..
	Recall that we defined a trace sink to print course change information from the
	mobility models of our simulation.  It should now be a lot more clear to you 
	what this function is doing.

Nós definimos um destino do rastreamento para imprimir a informação de mudança de rota dos modelos de mobilidade de nossa simulação. Agora está mais claro o que está função realizava.

::

  void
  CourseChange (std::string context, Ptr<const MobilityModel> model)
  {
    Vector position = model->GetPosition ();
    NS_LOG_UNCOND (context << 
      " x = " << position.x << ", y = " << position.y);
  }

..
	When we connected the "CourseChange" trace source to the above trace sink,
	we used what is called a "Config Path" to specify the source when we
	arranged a connection between the pre-defined trace source and the new trace 
	sink:

Quando conectamos a origem do rastreamento "CourseChange" para o destino do rastreamento anteriormente, usamos o que é chamado de caminho de configuração ("`Config Path`") para especificar a origem e o novo destino do rastreamento.

::

  std::ostringstream oss;
  oss <<
    "/NodeList/" << wifiStaNodes.Get (nWifi - 1)->GetId () <<
    "/$ns3::MobilityModel/CourseChange";

  Config::Connect (oss.str (), MakeCallback (&CourseChange));

..
	Let's try and make some sense of what is sometimes considered relatively
	mysterious code.  For the purposes of discussion, assume that the node 
	number returned by the ``GetId()`` is "7".  In this case, the path
	above turns out to be,

Para entendermos melhor o código, suponha que o número do nó retornado por ``GetId()`` é "7". Neste caso, o caminho seria,

::

  "/NodeList/7/$ns3::MobilityModel/CourseChange"

..
	The last segment of a config path must be an ``Attribute`` of an 
	``Object``.  In fact, if you had a pointer to the ``Object`` that has the
	"CourseChange" ``Attribute`` handy, you could write this just like we did 
	in the previous example.  You know by now that we typically store pointers to 
	our nodes in a NodeContainer.  In the ``third.cc`` example, the Nodes of
	interest are stored in the ``wifiStaNodes`` NodeContainer.  In fact, while
	putting the path together, we used this container to get a Ptr<Node> which we
	used to call GetId() on.  We could have used this Ptr<Node> directly to call
	a connect method directly:

O último segmento de um caminho de configuração deve ser um Atributo de um 
Objeto. Na verdade, se tínhamos um ponteiro para o Objeto que tem o Atributo
"CourseChange" ``, poderíamos escrever como no exemplo anterior.
Nós já sabemos que guardamos tipicamente ponteiros para outros nós em um ``NodeContainer``. No exemplo ``third.cc``, os nós de rede de interesse estão armazenados no ``wifiStaNodes`` ``NodeContainer``. De fato enquanto colocamos o caminho junto usamos este contêiner para obter um ``Ptr<Node>``, usado na chamada ``GetId()``. Poderíamos usar diretamente o ``Ptr<Node>`` para chamar um método de conexão.

::

  Ptr<Object> theObject = wifiStaNodes.Get (nWifi - 1);
  theObject->TraceConnectWithoutContext ("CourseChange", MakeCallback (&CourseChange));

..
	In the ``third.cc`` example, we actually want an additional "context" to 
	be delivered along with the Callback parameters (which will be explained below) so we 
	could actually use the following equivalent code,

No exemplo ``third.cc``, queremos um "contexto" adicional para ser encaminhado com os parâmetros do *callback* (os quais são explicados a seguir) então podemos usar o código equivalente,

::

  Ptr<Object> theObject = wifiStaNodes.Get (nWifi - 1);
  theObject->TraceConnect ("CourseChange", MakeCallback (&CourseChange));

..
	It turns out that the internal code for ``Config::ConnectWithoutContext`` and
	``Config::Connect`` actually do find a Ptr<Object> and call the appropriate
	TraceConnect method at the lowest level.

Acontece que o código interno para ``Config::ConnectWithoutContext`` e ``Config::Connect`` permite localizar um Ptr<Object> e chama o método ``TraceConnect``, no nível mais baixo.

..
	The ``Config`` functions take a path that represents a chain of ``Object`` 
	pointers.  Each segment of a path corresponds to an Object Attribute.  The last 
	segment is the Attribute of interest, and prior segments must be typed to contain
	or find Objects.  The ``Config`` code parses and "walks" this path until it 
	gets to the final segment of the path.  It then interprets the last segment as
	an ``Attribute`` on the last Object it found while walking the path.  The  
	``Config`` functions then call the appropriate ``TraceConnect`` or 
	``TraceConnectWithoutContext`` method on the final Object.  Let's see what 
	happens in a bit more detail when the above path is walked.

As funções ``Config`` aceitam um caminho que representa uma cadeia de ponteiros de Objetos. Cada segmento do caminho corresponde a um Atributo Objeto. O último segmento é o Atributo de interesse e os seguimentos anteriores devem ser definidos para conter ou encontrar Objetos. O  código ``Config`` processa o caminho até obter o segmento final. Então, interpreta o último segmento como um Atributo no último Objeto ele encontrou no caminho. Então as funções ``Config`` chamam o método ``TraceConnect`` ou ``TraceConnectWithoutContext`` adequado no Objeto final.

Vamos analisar com mais detalhes o processo descrito.

..
	The leading "/" character in the path refers to a so-called namespace.  One 
	of the predefined namespaces in the config system is "NodeList" which is a 
	list of all of the nodes in the simulation.  Items in the list are referred to
	by indices into the list, so "/NodeList/7" refers to the eighth node in the
	list of nodes created during the simulation.  This reference is actually a 
	``Ptr<Node>`` and so is a subclass of an ``ns3::Object``.  

O primeiro caractere "/" no caminho faz referência a um *namespace*. Um dos *namespaces* predefinidos no sistema de configuração é "NodeList" que é uma lista de todos os nós na simulação. Itens na lista são referenciados por índices , logo "/NodeList/7" refere-se ao oitavo nó na lista de nós criados durante a simulação. Esta referência é um ``Ptr<Node>``, por consequência é uma subclasse de um ``ns3::Object``.

..
	As described in the Object Model section of the |ns3| manual, we support
	Object Aggregation.  This allows us to form an association between different 
	Objects without any programming.  Each Object in an Aggregation can be reached 
	from the other Objects.  

Como descrito na seção Modelo de Objeto do manual |ns3|, há suporte para Agregação de Objeto. Isto permite realizar associação entre diferentes Objetos sem qualquer programação. Cada Objeto em uma Agregação pode ser acessado a partir de outros Objetos.

..
	The next path segment being walked begins with the "$" character.  This 
	indicates to the config system that a ``GetObject`` call should be made 
	looking for the type that follows.  It turns out that the MobilityHelper used in 
	``third.cc`` arranges to Aggregate, or associate, a mobility model to each of 
	the wireless Nodes.  When you add the "$" you are asking for another Object that
	has presumably been previously aggregated.  You can think of this as switching
	pointers from the original Ptr<Node> as specified by "/NodeList/7" to its 
	associated mobility model --- which is of type "$ns3::MobilityModel".  If you
	are familiar with ``GetObject``, we have asked the system to do the following:

O próximo segmento no caminho inicia com o carácter "$". O cifrão indica ao sistema de configuração que uma chamada ``GetObject`` deveria ser realizada procurando o tipo especificado em seguida. É diferente do que o ``MobilityHelper`` usou em ``third.cc`` gerenciar a Agregação, ou associar, um modelo de mobilidade para cada dos nós de rede sem fio. Quando adicionamos o "$", significa que estamos pedindo por outro Objeto que tinha sido presumidamente agregado anteriormente. Podemos pensar nisso como ponteiro de comutação do ``Ptr<Node>`` original como especificado por "/NodeList/7" para os modelos de mobilidade associados --- quais são do tipo "$ns3::MobilityModel". Se estivermos familiarizados com ``GetObject``, solicitamos ao sistema para fazer o 
seguinte:

::

  Ptr<MobilityModel> mobilityModel = node->GetObject<MobilityModel> ()

..
	We are now at the last Object in the path, so we turn our attention to the 
	Attributes of that Object.  The ``MobilityModel`` class defines an Attribute 
	called "CourseChange".  You can see this by looking at the source code in
	``src/mobility/model/mobility-model.cc`` and searching for "CourseChange" in your
	favorite editor.  You should find,

Estamos no último Objeto do caminho e neste verificamos os Atributos daquele Objeto. A classe ``MobilityModel`` define um Atributo chamado "CourseChange". Observando o código fonte em ``src/mobility/model/mobility-model.cc`` e procurando por "CourseChange", encontramos,

::

  .AddTraceSource ("CourseChange",
                   "The value of the position and/or velocity vector changed",
                   MakeTraceSourceAccessor (&MobilityModel::m_courseChangeTrace))

.. 
	which should look very familiar at this point.  

o qual parece muito familiar neste momento.

..
	If you look for the corresponding declaration of the underlying traced variable 
	in ``mobility-model.h`` you will find

Se procurarmos por declarações semelhantes das variáveis rastreadas em ``mobility-model.h``
encontraremos,

::

  TracedCallback<Ptr<const MobilityModel> > m_courseChangeTrace;

..
	The type declaration ``TracedCallback`` identifies ``m_courseChangeTrace``
	as a special list of Callbacks that can be hooked using the Config functions 
	described above.

A declaração de tipo ``TracedCallback`` identifica ``m_courseChangeTrace`` como um lista especial de *callbacks* que pode ser ligada usando as funções de Configuração descritas anteriormente.

..
	The ``MobilityModel`` class is designed to be a base class providing a common
	interface for all of the specific subclasses.  If you search down to the end of 
	the file, you will see a method defined called ``NotifyCourseChange()``:

A classe ``MobilityModel`` é projetada para ser a classe base provendo uma interface comum para todas as subclasses. No final do arquivo, encontramos um método chamado ``NotifyCourseChange()``:

::

  void
  MobilityModel::NotifyCourseChange (void) const
  {
    m_courseChangeTrace(this);
  }

..
	Derived classes will call into this method whenever they do a course change to
	support tracing.  This method invokes ``operator()`` on the underlying 
	``m_courseChangeTrace``, which will, in turn, invoke all of the registered
	Callbacks, calling all of the trace sinks that have registered interest in the
	trace source by calling a Config function.

Classes derivadas chamarão este método toda vez que fizerem uma alteração na rota para 
suportar rastreamento. Este método invoca ``operator()`` em ``m_courseChangeTrace``, 
que invocará todos os *callbacks* registrados, chamando todos os *trace sinks* que tem 
interesse registrado na origem do rastreamento usando a função de Configuração.

..
	So, in the ``third.cc`` example we looked at, whenever a course change is 
	made in one of the ``RandomWalk2dMobilityModel`` instances installed, there
	will be a ``NotifyCourseChange()`` call which calls up into the 
	``MobilityModel`` base class.  As seen above, this invokes ``operator()``
	on ``m_courseChangeTrace``, which in turn, calls any registered trace sinks.
	In the example, the only code registering an interest was the code that provided
	the config path.  Therefore, the ``CourseChange`` function that was hooked 
	from Node number seven will be the only Callback called.

No exemplo ``third.cc`` nós vimos que sempre que uma mudança de rota é realizada em uma das instâncias ``RandomWalk2dMobilityModel`` instaladas, haverá uma chamada ``NotifyCourseChange()`` da classe base ``MobilityModel``. Como observado, isto invoca ``operator()`` em ``m_courseChangeTrace``, que por sua vez, chama qualquer destino do rastreamento registrados. No exemplo, o único código que registrou interesse foi aquele que forneceu o caminho de configuração. Consequentemente, a função ``CourseChange`` que foi ligado no Node de número sete será a única *callback* chamada.

..
	The final piece of the puzzle is the "context".  Recall that we saw an output 
	looking something like the following from ``third.cc``:

A peça final do quebra-cabeça é o "contexto". Lembre-se da saída de ``third.cc``:

::

  /NodeList/7/$ns3::MobilityModel/CourseChange x = 7.27897, y = 2.22677

..
	The first part of the output is the context.  It is simply the path through
	which the config code located the trace source.  In the case we have been looking at
	there can be any number of trace sources in the system corresponding to any number
	of nodes with mobility models.  There needs to be some way to identify which trace
	source is actually the one that fired the Callback.  An easy way is to request a 
	trace context when you ``Config::Connect``.

A primeira parte da saída é o contexto. É simplesmente o caminho pelo qual o código de configuração localizou a origem do rastreamento. No caso, poderíamos ter qualquer número de origens de rastreamento no sistema correspondendo a qualquer número de nós com modelos de mobilidade. É necessário uma maneira de identificar qual origem do rastreamento disparou o *callback*. Uma forma simples é solicitar um contexto de rastreamento quando é usado o ``Config::Connect``.

.. 
	How to Find and Connect Trace Sources, and Discover Callback Signatures

Como Localizar e Conectar Origens de Rastreamento, e Descobrir Assinaturas de *Callback*
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

..
	The first question that inevitably comes up for new users of the Tracing system is,
	"okay, I know that there must be trace sources in the simulation core, but how do
	I find out what trace sources are available to me"?  
	
	The second question is, "okay, I found a trace source, how do I figure out the
	config path to use when I connect to it"? 

	The third question is, "okay, I found a trace source, how do I figure out what 
	the return type and formal arguments of my callback function need to be"?

	The fourth question is, "okay, I typed that all in and got this incredibly bizarre
	error message, what in the world does it mean"?

As questões que inevitavelmente os novos usuários do sistema de Rastreamento fazem, são:

1. "Eu sei que existem origens do rastreamento no núcleo da simulação, mas como 
   eu descubro quais estão disponíveis para mim?"
2. "Eu encontrei uma origem do rastreamento, como eu defino o caminho de configuração para 
   usar quando eu conectar a origem?"
3. "Eu encontrei uma origem do rastreamento, como eu  defino o tipo de retorno e os 
   argumentos formais da minha função de *callback*?"
4. "Eu fiz tudo corretamente e obtive uma mensagem de erro bizarra, o que isso significa?"

.. 
	What Trace Sources are Available?

Quais Origens de Rastreamento são Disponibilizadas
++++++++++++++++++++++++++++++++++++++++++++++++++

..
	The answer to this question is found in the |ns3| Doxygen.  If you go to 
	the project web site, 
	`ns-3 project
	<http://www.nsnam.org>`_, you will find a link to "Documentation" in the navigation bar.  If you select this link, you will be
	taken to our documentation page. There 
	is a link to "Latest Release" that will take you to the documentation
	for the latest stable release of |ns3|.
	If you select the "API Documentation" link, you will be
	taken to the |ns3| API documentation page.

A resposta é encontrada no Doxygen do |ns3|. Acesse o sítio Web do projeto, `ns-3 project <http://www.nsnam.org>`_, em seguida, "Documentation" na barra de navegação. Logo após, "Latest Release" e "API Documentation".

Acesse o item "Modules" na documentação do NS-3. Agora, selecione o item "C++ Constructs Used by All Modules". Serão exibidos quatro tópicos extremamente úteis:

* The list of all trace sources
* The list of all attributes
* The list of all global values
* Debugging

..
	The list of interest to us here is "the list of all trace sources".  Go 
	ahead and select that link.  You will see, perhaps not too surprisingly, a
	list of all of the trace sources available in the |ns3| core.

Estamos interessados em "*the list of all trace sources*" - a lista de todas origens do rastreamento. Selecionando este item, é exibido uma lista com todas origens disponíveis no núcleo do |ns3|.

..
	As an example, scroll down to ``ns3::MobilityModel``.  You will find
	an entry for

Como exemplo, ``ns3::MobilityModel``, terá uma entrada para

::

  CourseChange: The value of the position and/or velocity vector changed 

..
	You should recognize this as the trace source we used in the ``third.cc``
	example.  Perusing this list will be helpful.

No caso, esta foi a origem do rastreamento usada no exemplo ``third.cc``, esta lista será muito útil.

.. 
	What String do I use to Connect?

Qual String eu uso para Conectar?
+++++++++++++++++++++++++++++++++

..
	The easiest way to do this is to grep around in the |ns3| codebase for someone
	who has already figured it out,  You should always try to copy someone else's
	working code before you start to write your own.  Try something like:

A forma mais simples é procurar na base de código do |ns3| por alguém que já fez uso do caminho de configuração que precisamos para ligar a fonte de rastreamento. Sempre deveríamos primeiro copiar um código que funciona antes de escrever nosso próprio código. Tente usar os comandos:

::

  find . -name '*.cc' | xargs grep CourseChange | grep Connect

..
	and you may find your answer along with working code.  For example, in this
	case, ``./ns-3-dev/examples/wireless/mixed-wireless.cc`` has something
	just waiting for you to use:

e poderemos encontrar um código operacional que atenda nossas necessidades. Por exemplo, neste caso, ``./ns-3-dev/examples/wireless/mixed-wireless.cc`` tem algo que podemos usar:

::

  Config::Connect ("/NodeList/*/$ns3::MobilityModel/CourseChange", 
    MakeCallback (&CourseChangeCallback));

..
	If you cannot find any examples in the distribution, you can find this out
	from the |ns3| Doxygen.  It will probably be simplest just to walk 
	through the "CourseChanged" example.

Se não localizamos nenhum exemplo na distribuição, podemos tentar o Doxygen do |ns3|. É provavelmente mais simples que percorrer o exemplo "CourseChanged".

..
	Let's assume that you have just found the "CourseChanged" trace source in 
	"The list of all trace sources" and you want to figure out how to connect to
	it.  You know that you are using (again, from the ``third.cc`` example) an
	``ns3::RandomWalk2dMobilityModel``.  So open the "Class List" book in
	the NS-3 documentation tree by clicking its "+" box.  You will now see a
	list of all of the classes in |ns3|.  Scroll down until you see the
	entry for ``ns3::RandomWalk2dMobilityModel`` and follow that link.
	You should now be looking at the "ns3::RandomWalk2dMobilityModel Class 
	Reference".

Suponha que encontramos a origem do rastreamento "CourseChanged" na "The list of all trace sources" e queremos resolver como nos conectar a ela. Você sabe que está usando um ``ns3::RandomWalk2dMobilityModel``. Logo, acesse o item "Class List" na documentação do |ns3|. Será exibida a lista de todas as classes. Selecione a entrada para ``ns3::RandomWalk2dMobilityModel`` para exibir a documentação da classe.

..
	If you now scroll down to the "Member Function Documentation" section, you
	will see documentation for the ``GetTypeId`` function.  You constructed one
	of these in the simple tracing example above:

Acesse a seção "Member Function Documentation" e obterá a documentação para a função ``GetTypeId``. Você construiu uma dessas em um exemplo anterior:
	
::

    static TypeId GetTypeId (void)
    {
      static TypeId tid = TypeId ("MyObject")
        .SetParent (Object::GetTypeId ())
        .AddConstructor<MyObject> ()
        .AddTraceSource ("MyInteger",
                         "An integer value to trace.",
                         MakeTraceSourceAccessor (&MyObject::m_myInt))
        ;
      return tid;
    }

..
	As mentioned above, this is the bit of code that connected the Config 
	and Attribute systems to the underlying trace source.  This is also the
	place where you should start looking for information about the way to 
	connect. 

Como abordado, este código conecta os sistemas *Config* e Atributos à origem do rastreamento. É também o local onde devemos iniciar a busca por informação sobre como conectar.

..
	You are looking at the same information for the RandomWalk2dMobilityModel; and
	the information you want is now right there in front of you in the Doxygen:

Você está observando a mesma informação para ``RandomWalk2dMobilityModel``; e a informação que você precisa está explícita no Doxygen:
	
::

  This object is accessible through the following paths with Config::Set 
  		and Config::Connect: 

  /NodeList/[i]/$ns3::MobilityModel/$ns3::RandomWalk2dMobilityModel 

..
	The documentation tells you how to get to the ``RandomWalk2dMobilityModel`` 
	Object.  Compare the string above with the string we actually used in the 
	example code:

A documentação apresenta como obter o Objeto ``RandomWalk2dMobilityModel``. Compare o texto anterior com o texto que nós usamos no código do exemplo:

::

  "/NodeList/7/$ns3::MobilityModel"

..
	The difference is due to the fact that two ``GetObject`` calls are implied 
	in the string found in the documentation.  The first, for ``$ns3::MobilityModel``
	will query the aggregation for the base class.  The second implied 
	``GetObject`` call, for ``$ns3::RandomWalk2dMobilityModel``, is used to "cast"
	the base class to the concrete implementation class.  The documentation shows 
	both of these operations for you.  It turns out that the actual Attribute you are
	going to be looking for is found in the base class as we have seen.

A diferença é que há duas chamadas ``GetObject`` inclusas no texto da documentação. A primeira, para ``$ns3::MobilityModel`` solicita a agregação para a classe base. A segunda, para ``$ns3::RandomWalk2dMobilityModel`` é usada como *cast* da classe base para a 
implementação concreta da classe.  

.. 
	Look further down in the ``GetTypeId`` doxygen.  You will find,

Analisando ainda mais o ``GetTypeId`` no Doxygen, temos

::

  No TraceSources defined for this type.
  TraceSources defined in parent class ns3::MobilityModel:

  CourseChange: The value of the position and/or velocity vector changed 
  Reimplemented from ns3::MobilityModel

..
	This is exactly what you need to know.  The trace source of interest is found in
	``ns3::MobilityModel`` (which you knew anyway).  The interesting thing this
	bit of Doxygen tells you is that you don't need that extra cast in the config
	path above to get to the concrete class, since the trace source is actually in
	the base class.  Therefore the additional ``GetObject`` is not required and
	you simply use the path:

Isto é exatamente o que precisamos saber. A origem do rastreamento de interesse é encontrada em ``ns3::MobilityModel``.  O interessante é que pela documentação não é necessário o *cast* extra para obter a classe concreta, pois a origem do rastreamento está na classe base. Por consequência, o ``GetObject`` adicional não é necessário e podemos usar o caminho:

::

  /NodeList/[i]/$ns3::MobilityModel

.. 
	which perfectly matches the example path:

que é idêntico ao caminho do exemplo:

::

  /NodeList/7/$ns3::MobilityModel

.. 
	What Return Value and Formal Arguments?

Quais são os Argumentos Formais e o Valor de Retorno?
+++++++++++++++++++++++++++++++++++++++++++++++++++++

..
	The easiest way to do this is to grep around in the |ns3| codebase for someone
	who has already figured it out,  You should always try to copy someone else's
	working code.  Try something like:

A forma mais simples é procurar na base de código do |ns3| por um código existente. Você sempre deveria primeiro copiar um código que funciona antes de escrever seu próprio. Tente usar os comandos:
	
::

  find . -name '*.cc' | xargs grep CourseChange | grep Connect

..
	and you may find your answer along with working code.  For example, in this
	case, ``./ns-3-dev/examples/wireless/mixed-wireless.cc`` has something
	just waiting for you to use.  You will find

e você poderá encontrar código operacional. Por exemplo, neste caso, ``./ns-3-dev/examples/wireless/mixed-wireless.cc`` tem código para ser reaproveitado.

::

  Config::Connect ("/NodeList/*/$ns3::MobilityModel/CourseChange", 
    MakeCallback (&CourseChangeCallback));

..
	as a result of your grep.  The ``MakeCallback`` should indicate to you that
	there is a callback function there which you can use.  Sure enough, there is:

como resultado, ``MakeCallback`` indicaria que há uma função *callback* que pode ser usada.
Para reforçar:

::

  static void
  CourseChangeCallback (std::string path, Ptr<const MobilityModel> model)
  {
    ...
  }

.. 
	Take my Word for It

Acredite em Minha Palavra
~~~~~~~~~~~~~~~~~~~~~~~~~

..
	If there are no examples to work from, this can be, well, challenging to 
	actually figure out from the source code.

Se não há exemplos, pode ser desafiador descobrir por meio da análise do código fonte.

..
	Before embarking on a walkthrough of the code, I'll be kind and just tell you
	a simple way to figure this out:  The return value of your callback will always 
	be void.  The formal parameter list for a ``TracedCallback`` can be found 
	from the template parameter list in the declaration.  Recall that for our
	current example, this is in ``mobility-model.h``, where we have previously
	found:

Antes de aventurar-se no código, diremos algo importante: O valor de retorno de sua *callback* sempre será *void*. A lista de parâmetros formais para uma ``TracedCallback`` pode ser encontrada no lista de parâmetro padrão na declaração. Recorde do exemplo atual, isto está em ``mobility-model.h``, onde encontramos:

::

  TracedCallback<Ptr<const MobilityModel> > m_courseChangeTrace;

..
	There is a one-to-one correspondence between the template parameter list in 
	the declaration and the formal arguments of the callback function.  Here,
	there is one template parameter, which is a ``Ptr<const MobilityModel>``.
	This tells you that you need a function that returns void and takes a
	a ``Ptr<const MobilityModel>``.  For example,

Não há uma correspondência de um-para-um entre a lista de parâmetro padrão na declaração e os argumentos formais da função *callback*. Aqui, há um parâmetro padrão, que é um ``Ptr<const MobilityModel>``. Isto significa que precisamos de uma função que retorna *void* e possui um parâmetro ``Ptr<const MobilityModel>``. Por exemplo,

::

  void
  CourseChangeCallback (Ptr<const MobilityModel> model)
  {
    ...
  }

..
	That's all you need if you want to ``Config::ConnectWithoutContext``.  If
	you want a context, you need to ``Config::Connect`` and use a Callback 
	function that takes a string context, then the required argument.

Isto é tudo que precisamos para ``Config::ConnectWithoutContext``. Se você quer um contexto, use ``Config::Connect`` e uma função *callback* que possui como um parâmetro uma `string` de contexto, seguido pelo argumento.

::

  void
  CourseChangeCallback (std::string path, Ptr<const MobilityModel> model)
  {
    ...
  }

..
	If you want to ensure that your ``CourseChangeCallback`` is only visible
	in your local file, you can add the keyword ``static`` and come up with:

Para garantir que ``CourseChangeCallback`` seja somente visível em seu arquivo, você pode adicionar a palavra chave ``static``, como no exemplo:

::

  static void
  CourseChangeCallback (std::string path, Ptr<const MobilityModel> model)
  {
    ...
  }

.. 
	which is exactly what we used in the ``third.cc`` example.

exatamente o que é usado no exemplo ``third.cc``.

..
	The Hard Way

A Forma Complicada
~~~~~~~~~~~~~~~~~~

..
	This section is entirely optional.  It is going to be a bumpy ride, especially
	for those unfamiliar with the details of templates.  However, if you get through
	this, you will have a very good handle on a lot of the |ns3| low level
	idioms.

Esta seção é opcional. Pode ser bem penosa para aqueles que conhecem poucos detalhes de tipos parametrizados de dados (*templates*). Entretanto, se continuarmos nessa seção, mergulharemos em detalhes de baixo nível do |ns3|.

..
	So, again, let's figure out what signature of callback function is required for
	the "CourseChange" Attribute.  This is going to be painful, but you only need
	to do this once.  After you get through this, you will be able to just look at
	a ``TracedCallback`` and understand it.

Vamos novamente descobrir qual assinatura da função de *callback* é necessária para o Atributo "CourseChange". Isto pode ser doloroso, mas precisamos fazê-lo apenas uma vez. Depois de tudo, você será capaz de entender um ``TracedCallback``.

..
	The first thing we need to look at is the declaration of the trace source.
	Recall that this is in ``mobility-model.h``, where we have previously
	found:

Primeiro, verificamos a declaração da origem do rastreamento. Recorde que isto está em ``mobility-model.h``:
	
::

  TracedCallback<Ptr<const MobilityModel> > m_courseChangeTrace;

..
	This declaration is for a template.  The template parameter is inside the
	angle-brackets, so we are really interested in finding out what that
	``TracedCallback<>`` is.  If you have absolutely no idea where this might
	be found, grep is your friend.	

Esta declaração é para um *template*. O parâmetro do *template* está entre ``<>``, logo estamos interessados em descobrir o que é ``TracedCallback<>``. Se não tem nenhuma ideia de onde pode ser encontrado, use o utilitário *grep*.

..
	We are probably going to be interested in some kind of declaration in the 
	|ns3| source, so first change into the ``src`` directory.  Then, 
	we know this declaration is going to have to be in some kind of header file,
	so just grep for it using:

Estamos interessados em uma declaração similar no código fonte do |ns3|, logo buscamos no diretório ``src``. Então, sabemos que esta declaração tem um arquivo de cabeçalho, e procuramos por ele usando:

::

  find . -name '*.h' | xargs grep TracedCallback

..
	You'll see 124 lines fly by (I piped this through wc to see how bad it was).
	Although that may seem like it, that's not really a lot.  Just pipe the output
	through more and start scanning through it.  On the first page, you will see
	some very suspiciously template-looking stuff.

Obteremos 124 linhas, com este comando. Analisando a saída, encontramos alguns *templates* que podem ser úteis.

::

  TracedCallback<T1,T2,T3,T4,T5,T6,T7,T8>::TracedCallback ()
  TracedCallback<T1,T2,T3,T4,T5,T6,T7,T8>::ConnectWithoutContext (c ...
  TracedCallback<T1,T2,T3,T4,T5,T6,T7,T8>::Connect (const CallbackB ...
  TracedCallback<T1,T2,T3,T4,T5,T6,T7,T8>::DisconnectWithoutContext ...
  TracedCallback<T1,T2,T3,T4,T5,T6,T7,T8>::Disconnect (const Callba ...
  TracedCallback<T1,T2,T3,T4,T5,T6,T7,T8>::operator() (void) const ...
  TracedCallback<T1,T2,T3,T4,T5,T6,T7,T8>::operator() (T1 a1) const ...
  TracedCallback<T1,T2,T3,T4,T5,T6,T7,T8>::operator() (T1 a1, T2 a2 ...
  TracedCallback<T1,T2,T3,T4,T5,T6,T7,T8>::operator() (T1 a1, T2 a2 ...
  TracedCallback<T1,T2,T3,T4,T5,T6,T7,T8>::operator() (T1 a1, T2 a2 ...
  TracedCallback<T1,T2,T3,T4,T5,T6,T7,T8>::operator() (T1 a1, T2 a2 ...
  TracedCallback<T1,T2,T3,T4,T5,T6,T7,T8>::operator() (T1 a1, T2 a2 ...
  TracedCallback<T1,T2,T3,T4,T5,T6,T7,T8>::operator() (T1 a1, T2 a2 ...

..
	It turns out that all of this comes from the header file 
	``traced-callback.h`` which sounds very promising.  You can then take a
	look at ``mobility-model.h`` and see that there is a line which confirms
	this hunch:

Observamos que todas linhas são do arquivo de cabeçalho ``traced-callback.h``, logo ele parece muito promissor. Para confirmar, verifique o arquivo ``mobility-model.h``  e procure uma linha que corrobore esta suspeita.

::

  #include "ns3/traced-callback.h"

..
	Of course, you could have gone at this from the other direction and started
	by looking at the includes in ``mobility-model.h`` and noticing the 
	include of ``traced-callback.h`` and inferring that this must be the file
	you want.

Observando as inclusões em ``mobility-model.h``, verifica-se a inclusão do ``traced-callback.h`` e conclui-se que este deve ser o arquivo.

..
	In either case, the next step is to take a look at ``src/core/model/traced-callback.h``
	in your favorite editor to see what is happening.

O próximo passo é analisar o arquivo ``src/core/model/traced-callback.h`` e entender sua funcionalidade.

.. 
	You will see a comment at the top of the file that should be comforting:

Há um comentário no topo do arquivo que deveria ser animador:

::

  An ns3::TracedCallback has almost exactly the same API as a normal ns3::Callback but
  instead of forwarding calls to a single function (as an ns3::Callback normally does),
  it forwards calls to a chain of ns3::Callback.

.. 
	This should sound very familiar and let you know you are on the right track.

Isto deveria ser familiar e confirma que estamos no caminho correto.

.. 
	Just after this comment, you will find,

Depois deste comentário, encontraremos

::

  template<typename T1 = empty, typename T2 = empty, 
           typename T3 = empty, typename T4 = empty,
           typename T5 = empty, typename T6 = empty,
           typename T7 = empty, typename T8 = empty>
  class TracedCallback 
  {
    ...

..
	This tells you that TracedCallback is a templated class.  It has eight possible
	type parameters with default values.  Go back and compare this with the 
	declaration you are trying to understand:

Isto significa que TracedCallback é uma classe genérica (*templated class*). Possui oito possíveis tipos de parâmetros com valores padrões. Retorne e compare com a declaração que você está tentando entender:

::

  TracedCallback<Ptr<const MobilityModel> > m_courseChangeTrace;

..
	The ``typename T1`` in the templated class declaration corresponds to the 
	``Ptr<const MobilityModel>`` in the declaration above.  All of the other
	type parameters are left as defaults.  Looking at the constructor really
	doesn't tell you much.  The one place where you have seen a connection made
	between your Callback function and the tracing system is in the ``Connect``
	and ``ConnectWithoutContext`` functions.  If you scroll down, you will see
	a ``ConnectWithoutContext`` method here:

O ``typename T1`` na declaração da classe corresponde a ``Ptr<const MobilityModel>`` da declaração anterior. Todos os outros parâmetros são padrões. Observe que o construtor não contribui com muita informação. O único lugar onde há uma conexão entre a função *callback* e o sistema de rastreamento é nas funções ``Connect`` e ``ConnectWithoutContext``. Como mostrado a seguir:
	
::

  template<typename T1, typename T2, 
           typename T3, typename T4,
           typename T5, typename T6,
           typename T7, typename T8>
  void 
  TracedCallback<T1,T2,T3,T4,T5,T6,T7,T8>::ConnectWithoutContext ...
  {
    Callback<void,T1,T2,T3,T4,T5,T6,T7,T8> cb;
    cb.Assign (callback);
    m_callbackList.push_back (cb);
  }

..
	You are now in the belly of the beast.  When the template is instantiated for
	the declaration above, the compiler will replace ``T1`` with 
	``Ptr<const MobilityModel>``.  

Você está no olho do furação. Quando o *template* é instanciado pela declaração anterior, o compilador substitui ``T1`` por ``Ptr<const MobilityModel>``.

::

  void 
  TracedCallback<Ptr<const MobilityModel>::ConnectWithoutContext ... cb
  {
    Callback<void, Ptr<const MobilityModel> > cb;
    cb.Assign (callback);
    m_callbackList.push_back (cb);
  }

..
	You can now see the implementation of everything we've been talking about.  The
	code creates a Callback of the right type and assigns your function to it.  This
	is the equivalent of the ``pfi = MyFunction`` we discussed at the start of
	this section.  The code then adds the Callback to the list of Callbacks for 
	this source.  The only thing left is to look at the definition of Callback.
	Using the same grep trick as we used to find ``TracedCallback``, you will be
	able to find that the file ``./core/callback.h`` is the one we need to look at.

Podemos observar a implementação de tudo que foi explicado até este ponto. O código cria uma *callback* do tipo adequado e atribui sua função para ela. Isto é equivalente a ``pfi = MyFunction`` discutida anteriormente. O código então adiciona a *callback* para a lista de *callbacks* para esta origem. O que não observamos ainda é a definição da *callback*. Usando o utilitário *grep* podemos encontrar o arquivo ``./core/callback.h`` e verificar a definição.

..
	If you look down through the file, you will see a lot of probably almost
	incomprehensible template code.  You will eventually come to some Doxygen for
	the Callback template class, though.  Fortunately, there is some English:

No arquivo há muito código incompreensível. Felizmente há algum em Inglês. 

::

  This class template implements the Functor Design Pattern.
  It is used to declare the type of a Callback:
   - the first non-optional template argument represents
     the return type of the callback.
   - the second optional template argument represents
     the type of the first argument to the callback.
   - the third optional template argument represents
     the type of the second argument to the callback.
   - the fourth optional template argument represents
     the type of the third argument to the callback.
   - the fifth optional template argument represents
     the type of the fourth argument to the callback.
   - the sixth optional template argument represents
     the type of the fifth argument to the callback.

.. 
	We are trying to figure out what the

Nós estamos tentando descobrir o que significa a declaração 

::

    Callback<void, Ptr<const MobilityModel> > cb;

..
	declaration means.  Now we are in a position to understand that the first
	(non-optional) parameter, ``void``, represents the return type of the 
	Callback.  The second (non-optional) parameter, ``Ptr<const MobilityModel>``
	represents the first argument to the callback.

Agora entendemos que o primeiro parâmetro, ``void``, indica o tipo de retorno da *callback*. O segundo parâmetro, ``Ptr<const MobilityModel>`` representa o primeiro argumento da *callback*.

..
	The Callback in question is your function to receive the trace events.  From
	this you can infer that you need a function that returns ``void`` and takes
	a ``Ptr<const MobilityModel>``.  For example,

A *callback* em questão é a sua função que recebe os eventos de rastreamento. Logo, podemos deduzir que precisamos de uma função que retorna ``void`` e possui um parâmetro ``Ptr<const MobilityModel>``. Por exemplo,

::

  void
  CourseChangeCallback (Ptr<const MobilityModel> model)
  {
    ...
  }

..
	That's all you need if you want to ``Config::ConnectWithoutContext``.  If
	you want a context, you need to ``Config::Connect`` and use a Callback 
	function that takes a string context.  This is because the ``Connect``
	function will provide the context for you.  You'll need:

Isto é tudo que precisamos no ``Config::ConnectWithoutContext``. Se você quer um contexto, use ``Config::Connect`` e uma função *callback* que possui como um parâmetro uma `string` de contexto, seguido pelo argumento.

::

  void
  CourseChangeCallback (std::string path, Ptr<const MobilityModel> model)
  {
    ...
  }

..
	If you want to ensure that your ``CourseChangeCallback`` is only visible
	in your local file, you can add the keyword ``static`` and come up with:

Se queremos garantir que ``CourseChangeCallback`` é visível somente 
em seu arquivo, você pode adicionar a palavra chave ``static``, como no exemplo:

::

  static void
  CourseChangeCallback (std::string path, Ptr<const MobilityModel> model)
  {
    ...
  }

..
	which is exactly what we used in the ``third.cc`` example.  Perhaps you
	should now go back and reread the previous section (Take My Word for It).

o que é exatamente usado no exemplo ``third.cc``. Talvez seja interessante reler a seção (Acredite em Minha Palavra).

..
	If you are interested in more details regarding the implementation of 
	Callbacks, feel free to take a look at the |ns3| manual.  They are one
	of the most frequently used constructs in the low-level parts of |ns3|.
	It is, in my opinion, a quite elegant thing.

Há mais detalhes sobre a implementação de *callbacks* no manual do |ns3|. Elas estão entre os mais usados construtores das partes de baixo-nível do |ns3|. Em minha opinião, algo bastante elegante.

.. 
	What About TracedValue?

E quanto a TracedValue?
+++++++++++++++++++++++

..
	Earlier in this section, we presented a simple piece of code that used a
	``TracedValue<int32_t>`` to demonstrate the basics of the tracing code.
	We just glossed over the way to find the return type and formal arguments
	for the ``TracedValue``.  Rather than go through the whole exercise, we
	will just point you at the correct file, ``src/core/model/traced-value.h`` and
	to the important piece of code:

No início desta seção, nós apresentamos uma parte de código simples que usou um ``TracedValue<int32_t>`` para demonstrar o básico sobre código de rastreamento. Nós desprezamos os métodos para encontrar o tipo de retorno e os argumentos formais para o ``TracedValue``. Acelerando o processo, indicamos o arquivo ``src/core/model/traced-value.h`` e a parte relevante do código:

::

  template <typename T>
  class TracedValue
  {
  public:
    ...
    void Set (const T &v) {
      if (m_v != v)
        {
  	m_cb (m_v, v);
  	m_v = v;
        }
    }
    ...
  private:
    T m_v;
    TracedCallback<T,T> m_cb;
  };

..
	Here you see that the ``TracedValue`` is templated, of course.  In the simple
	example case at the start of the section, the typename is int32_t.  This means 
	that the member variable being traced (``m_v`` in the private section of the 
	class) will be an ``int32_t m_v``.  The ``Set`` method will take a 
	``const int32_t &v`` as a parameter.  You should now be able to understand 
	that the ``Set`` code will fire the ``m_cb`` callback with two parameters:
	the first being the current value of the ``TracedValue``; and the second 
	being the new value being set.

Verificamos que ``TracedValue`` é uma classe parametrizada. No caso simples do início da seção, o nome do tipo é int32_t. Isto significa que  a variável membro sendo rastreada (``m_v`` na seção privada da classe) será ``int32_t m_v``. O método ``Set`` possui um argumento ``const int32_t &v``. Você deveria ser capaz de entender que o código ``Set`` dispará o *callback* ``m_cb`` com dois parâmetros: o primeiro sendo o valor atual do ``TracedValue``; e o segundo sendo o novo valor.

..
	The callback, ``m_cb`` is declared as a ``TracedCallback<T, T>`` which
	will correspond to a ``TracedCallback<int32_t, int32_t>`` when the class is 
	instantiated.

A *callback* ``m_cb`` é declarada como um ``TracedCallback<T, T>`` que corresponderá a um ``TracedCallback<int32_t, int32_t>`` quando a classe é instanciada.

..
	Recall that the callback target of a TracedCallback always returns ``void``.  
	Further recall that there is a one-to-one correspondence between the template 
	parameter list in the declaration and the formal arguments of the callback 
	function.  Therefore the callback will need to have a function signature that 
	looks like:

Lembre-se que o destino da *callback* de um TracedCallback sempre retorna ``void``. Lembre também que há uma correspondência de um-para-um entre a lista de parâmetros polimórfica e os argumentos formais da função *callback*. Logo, a *callback* precisa ter uma assinatura de função similar a:

::

  void
  MyCallback (int32_t oldValue, int32_t newValue)
  {
    ...
  }

..
	It probably won't surprise you that this is exactly what we provided in that 
	simple example we covered so long ago:

Isto é exatamente o que nós apresentamos no exemplo simples abordado anteriormente.

::

  void
  IntTrace (int32_t oldValue, int32_t newValue)
  {
    std::cout << "Traced " << oldValue << " to " << newValue << std::endl;
  }

.. 
	A Real Example

Um Exemplo Real
***************

..
	Let's do an example taken from one of the best-known books on TCP around.  
	"TCP/IP Illustrated, Volume 1: The Protocols," by W. Richard Stevens is a 
	classic.  I just flipped the book open and ran across a nice plot of both the 
	congestion window and sequence numbers versus time on page 366.  Stevens calls 
	this, "Figure 21.10. Value of cwnd and send sequence number while data is being 
	transmitted."  Let's just recreate the cwnd part of that plot in |ns3|
	using the tracing system and ``gnuplot``.

Vamos fazer um exemplo retirado do livro "TCP/IP Illustrated, Volume 1: The Protocols" escrito por W. Richard Stevens. Localizei na página 366 do livro um gráfico da janela de congestionamento e números de sequência versus tempo. Stevens denomina de "Figure 21.10. Value of cwnd and send sequence number while data is being transmitted." Vamos recriar a parte *cwnd* daquele gráfico em |ns3| usando o sistema de rastreamento e ``gnuplot``.

.. 
	Are There Trace Sources Available?

Há Fontes de Rastreamento Disponibilizadas?
+++++++++++++++++++++++++++++++++++++++++++

..
	The first thing to think about is how we want to get the data out.  What is it
	that we need to trace?  The first thing to do is to consult "The list of all
	trace sources" to see what we have to work with.  Recall that this is found
	in the |ns3| Doxygen in the "C++ Constructs Used by All Modules" Module section.  If you scroll
	through the list, you will eventually find:

Primeiro devemos pensar sobre como queremos obter os dados de saída. O que é que  nós precisamos rastrear? Consultamos então *"The list of all trace sources"* para sabermos o que temos para trabalhar. Essa seção encontra-se na documentação na seção *"Module"*, no item *"C++ Constructs Used by All Modules"*. Procurando na lista, encontraremos:

::

  ns3::TcpNewReno
  CongestionWindow: The TCP connection's congestion window

..
	It turns out that the |ns3| TCP implementation lives (mostly) in the 
	file ``src/internet/model/tcp-socket-base.cc`` while congestion control
	variants are in files such as ``src/internet/model/tcp-newreno.cc``.  
	If you don't know this a priori, you can use the recursive grep trick:

A maior parte da implementação do TCP no |ns3| está no arquivo ``src/internet/model/tcp-socket-base.cc`` enquanto variantes do controle de congestionamento estão em arquivos como ``src/internet/model/tcp-newreno.cc``. Se não sabe a priori dessa informação, use:

::

  find . -name '*.cc' | xargs grep -i tcp

.. 
	You will find page after page of instances of tcp pointing you to that file. 

Haverá páginas de respostas apontando para aquele arquivo.

..
	If you open ``src/internet/model/tcp-newreno.cc`` in your favorite 
	editor, you will see right up at the top of the file, the following declarations:

No início do arquivo ``src/internet/model/tcp-newreno.cc`` há as seguintes declarações:

::

  TypeId
  TcpNewReno::GetTypeId ()
  {
    static TypeId tid = TypeId("ns3::TcpNewReno")
      .SetParent<TcpSocketBase> ()
      .AddConstructor<TcpNewReno> ()
      .AddTraceSource ("CongestionWindow",
                       "The TCP connection's congestion window",
                       MakeTraceSourceAccessor (&TcpNewReno::m_cWnd))
      ;
    return tid;
  }

..
	This should tell you to look for the declaration of ``m_cWnd`` in the header
	file ``src/internet/model/tcp-newreno.h``.  If you open this file in your
	favorite editor, you will find:

Isto deveria guiá-lo para localizar a declaração de ``m_cWnd`` no arquivo de cabeçalho ``src/internet/model/tcp-newreno.h``. Temos nesse arquivo:

::

  TracedValue<uint32_t> m_cWnd; //Congestion window

..
	You should now understand this code completely.  If we have a pointer to the 
	``TcpNewReno``, we can ``TraceConnect`` to the "CongestionWindow" trace 
	source if we provide an appropriate callback target.  This is the same kind of
	trace source that we saw in the simple example at the start of this section,
	except that we are talking about ``uint32_t`` instead of ``int32_t``.

Você deveria entender este código. Se nós temos um ponteiro para ``TcpNewReno``, podemos fazer ``TraceConnect`` para a origem do rastreamento "CongestionWindow" se fornecermos uma *callback* adequada. É o mesmo tipo de origem do rastreamento que nós abordamos no exemplo simples no início da seção, exceto que estamos usando ``uint32_t`` ao invés de ``int32_t``.

..
	We now know that we need to provide a callback that returns void and takes 
	two ``uint32_t`` parameters, the first being the old value and the second 
	being the new value:

Precisamos prover uma *callback* que retorne ``void`` e receba dois parâmetros ``uint32_t``, o primeiro representando o valor antigo e o segundo o novo valor:

::

  void
  CwndTrace (uint32_t oldValue, uint32_t newValue)
  {
    ...
  }

.. 
	What Script to Use?

Qual código Usar?
+++++++++++++++++

..
	It's always best to try and find working code laying around that you can 
	modify, rather than starting from scratch.  So the first order of business now
	is to find some code that already hooks the "CongestionWindow" trace source
	and see if we can modify it.  As usual, grep is your friend:

É sempre melhor localizar e modificar um código operacional que iniciar do zero. Portanto, vamos procurar uma origem do rastreamento da "CongestionWindow" e verificar se é possível modificar. Para tal, usamos novamente o *grep*:

::

  find . -name '*.cc' | xargs grep CongestionWindow

..
	This will point out a couple of promising candidates: 
	``examples/tcp/tcp-large-transfer.cc`` and 
	``src/test/ns3tcp/ns3tcp-cwnd-test-suite.cc``.

Encontramos alguns candidatos:
``examples/tcp/tcp-large-transfer.cc`` e
``src/test/ns3tcp/ns3tcp-cwnd-test-suite.cc``.

..
	We haven't visited any of the test code yet, so let's take a look there.  You
	will typically find that test code is fairly minimal, so this is probably a
	very good bet.  Open ``src/test/ns3tcp/ns3tcp-cwnd-test-suite.cc`` in your
	favorite editor and search for "CongestionWindow".  You will find,

Nós não visitamos nenhum código de teste ainda, então vamos fazer isto agora. Código de teste é pequeno, logo é uma ótima escolha. Acesse o arquivo ``src/test/ns3tcp/ns3tcp-cwnd-test-suite.cc`` e localize "CongestionWindow". Como resultado, temos

::

  ns3TcpSocket->TraceConnectWithoutContext ("CongestionWindow", 
    MakeCallback (&Ns3TcpCwndTestCase1::CwndChange, this));

..
	This should look very familiar to you.  We mentioned above that if we had a
	"CongestionWindow" trace source.  That's exactly what we have here; so it
	pointer to the ``TcpNewReno``, we could ``TraceConnect`` to the 
	turns out that this line of code does exactly what we want.  Let's go ahead
	and extract the code we need from this function 
	(``Ns3TcpCwndTestCase1::DoRun (void)``).  If you look at this function,
	you will find that it looks just like an |ns3| script.  It turns out that
	is exactly what it is.  It is a script run by the test framework, so we can just
	pull it out and wrap it in ``main`` instead of in ``DoRun``.  Rather than
	walk through this, step, by step, we have provided the file that results from
	porting this test back to a native |ns3| script --
	``examples/tutorial/fifth.cc``.  

Como abordado, temos uma origem do rastreamento  "CongestionWindow"; então ela aponta para ``TcpNewReno``, poderíamos alterar o ``TraceConnect`` para o que nós desejamos. Vamos extrair o código que precisamos desta função (``Ns3TcpCwndTestCase1::DoRun (void)``). Se você observar, perceberá que parece como um código |ns3|. E descobre-se exatamente que realmente é um código. É um código executado pelo `framework` de teste, logo podemos apenas colocá-lo no ``main`` ao invés de ``DoRun``.  A tradução deste teste para um código nativo do |ns3| é apresentada no arquivo ``examples/tutorial/fifth.cc``.


.. 
	A Common Problem and Solution

Um Problema Comum e a Solução
+++++++++++++++++++++++++++++

..
	The ``fifth.cc`` example demonstrates an extremely important rule that you 
	must understand before using any kind of ``Attribute``:  you must ensure 
	that the target of a ``Config`` command exists before trying to use it.
	This is no different than saying an object must be instantiated before trying
	to call it.  Although this may seem obvious when stated this way, it does
	trip up many people trying to use the system for the first time.

O exemplo ``fifth.cc`` demonstra um importante regra que devemos entender antes de usar qualquer tipo de  Atributo: devemos garantir que o alvo de um comando ``Config`` existe antes de tentar usá-lo. É a mesma ideia que um objeto não pode ser usado sem ser primeiro instanciado. Embora pareça óbvio, muitas pessoas erram ao usar o sistema pela primeira vez.

..
	Let's return to basics for a moment.  There are three basic time periods that
	exist in any |ns3| script.  The first time period is sometimes called 
	"Configuration Time" or "Setup Time," and is in force during the period 
	when the ``main`` function of your script is running, but before 
	``Simulator::Run`` is called.  The second time period  is sometimes called
	"Simulation Time" and is in force during the time period when 
	``Simulator::Run`` is actively executing its events.  After it completes
	executing the simulation,  ``Simulator::Run`` will return control back to 
	the ``main`` function.  When this happens, the script enters what can be 
	called "Teardown Time," which is when the structures and objects created 
	during setup and taken apart and released.

Há três fases básicas em qualquer código |ns3|. A primeira é a chamada de "Configuration Time" ou "Setup Time" e ocorre durante a execução da função ``main``, mas antes da chamada ``Simulator::Run``. O segunda fase é chamada de "Simulation Time" e é quando o ``Simulator::Run`` está executando seus eventos. Após completar a execução da simulação, ``Simulator::Run`` devolve o controle a função ``main``. Quando isto acontece, o código entra na terceira fase, o "Teardown Time", que  é quando estruturas e objetos criados durante a configuração são analisados e liberados.

..
	Perhaps the most common mistake made in trying to use the tracing system is 
	assuming that entities constructed dynamically during simulation time are
	available during configuration time.  In particular, an |ns3|
	``Socket`` is a dynamic object often created by ``Applications`` to
	communicate between ``Nodes``.  An |ns3| ``Application`` 
	always has a "Start Time" and a "Stop Time" associated with it.  In the
	vast majority of cases, an ``Application`` will not attempt to create 
	a dynamic object until its ``StartApplication`` method is called at some
	"Start Time".  This is to ensure that the simulation is completely 
	configured before the app tries to do anything (what would happen if it tried
	to connect to a node that didn't exist yet during configuration time). 
	The answer to this issue is to 1) create a simulator event that is run after the 
	dynamic object is created and hook the trace when that event is executed; or
	2) create the dynamic object at configuration time, hook it then, and give 
	the object to the system to use during simulation time.  We took the second 
	approach in the ``fifth.cc`` example.  This decision required us to create
	the ``MyApp`` ``Application``, the entire purpose of which is to take 
	a ``Socket`` as a parameter.  

Talvez o erro mais comum em tentar usar o sistema de rastreamento é supor que entidades construídas dinamicamente durante a fase de simulação estão acessíveis durante  a fase de configuração. Em particular, um ``Socket`` |ns3| é um objeto dinâmico frequentemente criado por Aplicações (``Applications``) para comunicação entre nós de redes.  Uma Aplicação |ns3| tem um "Start Time" e "Stop Time" associado a ela. Na maioria dos casos, uma Aplicação não tentar criar um objeto dinâmico até que seu método ``StartApplication`` é chamado
em algum "Start Time". Isto é para garantir que a simulação está completamente configurada antes que a aplicação tente fazer alguma coisa (o que aconteceria se tentasse conectar a um nó que não existisse durante a fase de configuração). A resposta para esta questão é:
	
1. criar um evento no simulador que é executado depois que o objeto dinâmico 
   é criado e ativar o rastreador quando aquele evento é executado; ou 
2. criar o objeto dinâmico na fase de configuração, ativá-lo,
   e passar o objeto para o sistema usar durante a fase de simulação. 

Nós consideramos a segunda abordagem no exemplo ``fifth.cc``. A decisão implicou na criação da Aplicação ``MyApp``, com o propósito de passar um ``Socket`` como parâmetro.

.. 
	A fifth.cc Walkthrough

Analisando o exemplo fifth.cc
+++++++++++++++++++++++++++++

..
	Now, let's take a look at the example program we constructed by dissecting
	the congestion window test.  Open ``examples/tutorial/fifth.cc`` in your
	favorite editor.  You should see some familiar looking code:

Agora, vamos analisar o programa exemplo detalhando o teste da janela de congestionamento.
Segue o código do arquivo localizado em ``examples/tutorial/fifth.cc``:

::

  /* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
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
   * Foundation, Include., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
   */
  
  #include <fstream>
  #include "ns3/core-module.h"
  #include "ns3/network-module.h"
  #include "ns3/internet-module.h"
  #include "ns3/point-to-point-module.h"
  #include "ns3/applications-module.h"
  
  using namespace ns3;
  
  NS_LOG_COMPONENT_DEFINE ("FifthScriptExample");

..
	This has all been covered, so we won't rehash it.  The next lines of source are
	the network illustration and a comment addressing the problem described above
	with ``Socket``.

Todo o código apresentado já foi discutido. As próximas linhas são comentários apresentando
a estrutura da rede e comentários abordando o problema descrito com o ``Socket``.

::

  // ===========================================================================
  //
  //         node 0                 node 1
  //   +----------------+    +----------------+
  //   |    ns-3 TCP    |    |    ns-3 TCP    |
  //   +----------------+    +----------------+
  //   |    10.1.1.1    |    |    10.1.1.2    |
  //   +----------------+    +----------------+
  //   | point-to-point |    | point-to-point |
  //   +----------------+    +----------------+
  //           |                     |
  //           +---------------------+
  //                5 Mbps, 2 ms
  //
  //
  // We want to look at changes in the ns-3 TCP congestion window.  We need
  // to crank up a flow and hook the CongestionWindow attribute on the socket
  // of the sender.  Normally one would use an on-off application to generate a
  // flow, but this has a couple of problems.  First, the socket of the on-off
  // application is not created until Application Start time, so we wouldn't be
  // able to hook the socket (now) at configuration time.  Second, even if we
  // could arrange a call after start time, the socket is not public so we
  // couldn't get at it.
  //
  // So, we can cook up a simple version of the on-off application that does what
  // we want.  On the plus side we don't need all of the complexity of the on-off
  // application.  On the minus side, we don't have a helper, so we have to get
  // a little more involved in the details, but this is trivial.
  //
  // So first, we create a socket and do the trace connect on it; then we pass
  // this socket into the constructor of our simple application which we then
  // install in the source node.
  // ===========================================================================
  //

.. 
	This should also be self-explanatory.  

..
	The next part is the declaration of the ``MyApp`` ``Application`` that
	we put together to allow the ``Socket`` to be created at configuration time.

A próxima parte é a declaração da Aplicação ``MyApp`` que permite que o ``Socket`` seja criado na fase de configuração.

::

  class MyApp : public Application
  {
  public:
  
    MyApp ();
    virtual ~MyApp();
  
    void Setup (Ptr<Socket> socket, Address address, uint32_t packetSize, 
      uint32_t nPackets, DataRate dataRate);
  
  private:
    virtual void StartApplication (void);
    virtual void StopApplication (void);
  
    void ScheduleTx (void);
    void SendPacket (void);
  
    Ptr<Socket>     m_socket;
    Address         m_peer;
    uint32_t        m_packetSize;
    uint32_t        m_nPackets;
    DataRate        m_dataRate;
    EventId         m_sendEvent;
    bool            m_running;
    uint32_t        m_packetsSent;
  };

..
	You can see that this class inherits from the |ns3| ``Application``
	class.  Take a look at ``src/network/model/application.h`` if you are interested in 
	what is inherited.  The ``MyApp`` class is obligated to override the 
	``StartApplication`` and ``StopApplication`` methods.  These methods are
	automatically called when ``MyApp`` is required to start and stop sending
	data during the simulation.

A classe ``MyApp`` herda a classe ``Application`` do |ns3|. Acesse  o arquivo ``src/network/model/application.h`` se estiver interessado sobre detalhes dessa herança. A classe ``MyApp`` é obrigada sobrescrever os métodos ``StartApplication`` e ``StopApplication``. Estes métodos são automaticamente chamado quando ``MyApp`` é solicitada iniciar e parar de enviar dados durante a simulação.

.. 
	How Applications are Started and Stopped (optional)

Como Aplicações são Iniciadas e Paradas (Opcional)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

..
	It is worthwhile to spend a bit of time explaining how events actually get 
	started in the system.  This is another fairly deep explanation, and can be
	ignored if you aren't planning on venturing down into the guts of the system.
	It is useful, however, in that the discussion touches on how some very important
	parts of |ns3| work and exposes some important idioms.  If you are 
	planning on implementing new models, you probably want to understand this
	section.

Nesta seção é explicado como eventos tem início no sistema. É uma explicação mais detalhada e não é necessária se não planeja entender detalhes do sistema. É interessante, por outro lado, pois aborda como partes do |ns3| trabalham e mostra alguns detalhes de implementação importantes. Se você planeja implementar novos modelos, então deve entender essa seção.

..
	The most common way to start pumping events is to start an ``Application``.
	This is done as the result of the following (hopefully) familar lines of an 
	|ns3| script:

A maneira mais comum de iniciar eventos é iniciar uma Aplicação. Segue as linhas de um código |ns3| que faz exatamente isso:

::

  ApplicationContainer apps = ...
  apps.Start (Seconds (1.0));
  apps.Stop (Seconds (10.0));

..
	The application container code (see ``src/network/helper/application-container.h`` if
	you are interested) loops through its contained applications and calls,

O código do contêiner aplicação (``src/network/helper/application-container.h``) itera pelas aplicações no contêiner e chama,

::

  app->SetStartTime (startTime);

.. 
	as a result of the ``apps.Start`` call and

como um resultado da chamada ``apps.Start`` e

::

  app->SetStopTime (stopTime);

.. 
	as a result of the ``apps.Stop`` call.

como um resultado da chamada  ``apps.Stop``.

..
	The ultimate result of these calls is that we want to have the simulator 
	automatically make calls into our ``Applications`` to tell them when to
	start and stop.  In the case of ``MyApp``, it inherits from class
	``Application`` and overrides ``StartApplication``, and 
	``StopApplication``.  These are the functions that will be called by
	the simulator at the appropriate time.  In the case of ``MyApp`` you
	will find that ``MyApp::StartApplication`` does the initial ``Bind``,
	and ``Connect`` on the socket, and then starts data flowing by calling
	``MyApp::SendPacket``.  ``MyApp::StopApplication`` stops generating
	packets by cancelling any pending send events and closing the socket.

O último resultado destas chamadas queremos ter o simulador executando chamadas em nossa ``Applications`` para controlar o inicio e a parada. No caso ``MyApp``, herda da classe ``Application`` e sobrescreve ``StartApplication`` e ``StopApplication``. Estas são as funções invocadas pelo simulador no momento certo. No caso de ``MyApp``, o ``MyApp::StartApplication`` faz o ``Bind`` e ``Connect`` no `socket`, em seguida, inicia o fluxo de dados chamando ``MyApp::SendPacket``. ``MyApp::StopApplication`` interrompe a geração de pacotes cancelando qualquer evento pendente de envio e também fechando o socket.

..
	One of the nice things about |ns3| is that you can completely 
	ignore the implementation details of how your ``Application`` is 
	"automagically" called by the simulator at the correct time.  But since
	we have already ventured deep into |ns3| already, let's go for it.

Uma das coisas legais sobre o |ns3| é que podemos ignorar completamente os detalhes de implementação de como sua Aplicação é "automaticamente" chamada pelo simulador no momento correto. De qualquer forma, detalhamos como isso acontece a seguir.

..
	If you look at ``src/network/model/application.cc`` you will find that the
	``SetStartTime`` method of an ``Application`` just sets the member 
	variable ``m_startTime`` and the ``SetStopTime`` method just sets 
	``m_stopTime``.  From there, without some hints, the trail will probably
	end.

Se observarmos em ``src/network/model/application.cc``, descobriremos que o 
método ``SetStartTime`` de uma ``Application`` apenas altera a variável ``m_startTime`` e o método ``SetStopTime`` apenas altera a variável ``m_stopTime``.  

..
	The key to picking up the trail again is to know that there is a global 
	list of all of the nodes in the system.  Whenever you create a node in 
	a simulation, a pointer to that node is added to the global ``NodeList``.

Para continuar e entender o processo, precisamos saber que há uma lista global de todos os nós no sistema. Sempre que você cria um nó em uma simulação, um ponteiro para aquele nó é adicionado para a lista global ``NodeList``.

..
	Take a look at ``src/network/model/node-list.cc`` and search for 
	``NodeList::Add``.  The public static implementation calls into a private
	implementation called ``NodeListPriv::Add``.  This is a relatively common
	idom in |ns3|.  So, take a look at ``NodeListPriv::Add``.  There
	you will find,

Observe em ``src/network/model/node-list.cc`` e procure por ``NodeList::Add``. A implementação ``public static`` chama uma implementação privada denominada ``NodeListPriv::Add``. Isto é comum no |ns3|. Então, observe ``NodeListPriv::Add`` e encontrará,

::

  Simulator::ScheduleWithContext (index, TimeStep (0), &Node::Start, node);

..
	This tells you that whenever a ``Node`` is created in a simulation, as
	a side-effect, a call to that node's ``Start`` method is scheduled for
	you that happens at time zero.  Don't read too much into that name, yet.
	It doesn't mean that the node is going to start doing anything, it can be
	interpreted as an informational call into the ``Node`` telling it that 
	the simulation has started, not a call for action telling the ``Node``
	to start doing something.

Isto significa que sempre que um ``Node`` é criado em uma simulação, como uma implicação, uma chamada para o método ``Start`` do nó é agendada para que ocorra no tempo zero. Isto não significa que o nó vai iniciar fazendo alguma coisa, pode ser interpretado como uma chamada informacional no ``Node`` dizendo a ele que a simulação teve início, não uma chamada para ação dizendo ao ``Node`` iniciar alguma coisa.

..
	So, ``NodeList::Add`` indirectly schedules a call to ``Node::Start``
	at time zero to advise a new node that the simulation has started.  If you 
	look in ``src/network/model/node.h`` you will, however, not find a method called
	``Node::Start``.  It turns out that the ``Start`` method is inherited
	from class ``Object``.  All objects in the system can be notified when
	the simulation starts, and objects of class ``Node`` are just one kind
	of those objects.

Então, o ``NodeList::Add`` indiretamente agenda uma chamada para ``Node::Start`` no tempo zero, para informar ao novo nó que a simulação foi iniciada. Se olharmos em ``src/network/model/node.h`` não acharemos um método chamado ``Node::Start``. Acontece que o método ``Start`` é herdado da classe ``Object``. Todos objetos no sistema podem ser avisados que a simulação iniciou e objetos da classe ``Node`` são exemplos.

..
	Take a look at ``src/core/model/object.cc`` next and search for ``Object::Start``.
	This code is not as straightforward as you might have expected since 
	|ns3| ``Objects`` support aggregation.  The code in 
	``Object::Start`` then loops through all of the objects that have been
	aggregated together and calls their ``DoStart`` method.  This is another
	idiom that is very common in |ns3|.  There is a public API method,
	that stays constant across implementations, that calls a private implementation
	method that is inherited and implemented by subclasses.  The names are typically
	something like ``MethodName`` for the public API and ``DoMethodName`` for
	the private API.

Observe em seguida ``src/core/model/object.cc``. Localize por ``Object::Start``. Este código não é tão simples como você esperava desde que ``Objects`` |ns3| suportam agregação. O código em ``Object::Start`` então percorre todos os objetos que estão agregados e chama o método ``DoStart`` de cada um. Este é uma outra prática muito comum em |ns3|. Há um método pública na API, que permanece constante entre implementações, que chama um método de implementação privada que é herdado e implementado por subclasses. Os nomes são tipicamente
algo como ``MethodName`` para os da API pública e ``DoMethodName`` para os da API privada.

..
	This tells us that we should look for a ``Node::DoStart`` method in 
	``src/network/model/node.cc`` for the method that will continue our trail.  If you
	locate the code, you will find a method that loops through all of the devices
	in the node and then all of the applications in the node calling 
	``device->Start`` and ``application->Start`` respectively.

Logo, deveríamos procurar por um método ``Node::DoStart`` em  ``src/network/model/node.cc``. Ao localizar o método, descobrirá um método que percorre todos os dispositivos e aplicações no nó chamando respectivamente ``device->Start`` e ``application->Start``.

..
	You may already know that classes ``Device`` and ``Application`` both
	inherit from class ``Object`` and so the next step will be to look at
	what happens when ``Application::DoStart`` is called.  Take a look at
	``src/network/model/application.cc`` and you will find:

As classes ``Device`` e ``Application`` herdam da classe ``Object``, então o próximo passo é entender o que acontece quando ``Application::DoStart`` é executado. Observe o código em ``src/network/model/application.cc``:

::

  void
  Application::DoStart (void)
  {
    m_startEvent = Simulator::Schedule (m_startTime, &Application::StartApplication, this);
    if (m_stopTime != TimeStep (0))
      {
        m_stopEvent = Simulator::Schedule (m_stopTime, &Application::StopApplication, this);
      }
    Object::DoStart ();
  }

..
	Here, we finally come to the end of the trail.  If you have kept it all straight,
	when you implement an |ns3| ``Application``, your new application 
	inherits from class ``Application``.  You override the ``StartApplication``
	and ``StopApplication`` methods and provide mechanisms for starting and 
	stopping the flow of data out of your new ``Application``.  When a ``Node``
	is created in the simulation, it is added to a global ``NodeList``.  The act
	of adding a node to this ``NodeList`` causes a simulator event to be scheduled
	for time zero which calls the ``Node::Start`` method of the newly added 
	``Node`` to be called when the simulation starts.  Since a ``Node`` inherits
	from ``Object``, this calls the ``Object::Start`` method on the ``Node``
	which, in turn, calls the ``DoStart`` methods on all of the ``Objects``
	aggregated to the ``Node`` (think mobility models).  Since the ``Node``
	``Object`` has overridden ``DoStart``, that method is called when the 
	simulation starts.  The ``Node::DoStart`` method calls the ``Start`` methods
	of all of the ``Applications`` on the node.  Since ``Applications`` are
	also ``Objects``, this causes ``Application::DoStart`` to be called.  When
	``Application::DoStart`` is called, it schedules events for the 
	``StartApplication`` and ``StopApplication`` calls on the ``Application``.
	These calls are designed to start and stop the flow of data from the 
	``Application``

Aqui finalizamos nosso detalhamento. Ao implementar uma Aplicação do |ns3|, sua nova aplicação herda da classe ``Application``. Você sobrescreve os métodos ``StartApplication`` e ``StopApplication`` e provê mecanismos para iniciar e finalizar o fluxo de dados de sua nova ``Application``. Quando um ``Node`` é criado na simulação, ele é adicionado a uma lista global ``NodeList``. A ação de adicionar um nó na lista faz com que um evento do simulador seja agendado para o tempo zero e que chama o método ``Node::Start`` do ``Node`` recentemente adicionado para ser chamado quando a simulação inicia. Como um ``Node`` herda de ``Object``,
a chamada invoca o método ``Object::Start`` no ``Node``, o qual, por sua vez, chama os métodos ``DoStart`` em todos os ``Objects`` agregados ao ``Node`` (pense em modelos móveis).  Como o ``Node`` ``Object`` 
tem sobrescritos ``DoStart``, o método é chamado quando a simulação inicia. O método ``Node::DoStart`` chama o método ``Start`` de todas as ``Applications`` no nó. Por sua vez, ``Applications`` são também ``Objects``, o que resulta na invocação do ``Application::DoStart``.  Quando ``Application::DoStart`` é chamada, ela agenda eventos para as chamadas ``StartApplication`` e ``StopApplication`` na ``Application``. Estas chamadas são projetadas para iniciar e parar o fluxo de dados da ``Application``.

..
	This has been another fairly long journey, but it only has to be made once, and
	you now understand another very deep piece of |ns3|.

Após essa longa jornada, você pode entende melhor outra parte do |ns3|.

..
	The MyApp Application

A Aplicação MyApp
~~~~~~~~~~~~~~~~~

..
	The ``MyApp`` ``Application`` needs a constructor and a destructor,
	of course:

A Aplicação ``MyApp`` precisa de um construtor e um destrutor,

::

  MyApp::MyApp ()
    : m_socket (0),
      m_peer (),
      m_packetSize (0),
      m_nPackets (0),
      m_dataRate (0),
      m_sendEvent (),
      m_running (false),
      m_packetsSent (0)
  {
  }
  
  MyApp::~MyApp()
  {
    m_socket = 0;
  }

..
	The existence of the next bit of code is the whole reason why we wrote this
	``Application`` in the first place.

O código seguinte é a principal razão da existência desta Aplicação.

::

  void
  MyApp::Setup (Ptr<Socket> socket, Address address, uint32_t packetSize, 
                       uint32_t nPackets, DataRate dataRate)
  {
    m_socket = socket;
    m_peer = address;
    m_packetSize = packetSize;
    m_nPackets = nPackets;
    m_dataRate = dataRate;
  }

..  
	This code should be pretty self-explanatory.  We are just initializing member
	variables.  The important one from the perspective of tracing is the 
	``Ptr<Socket> socket`` which we needed to provide to the application 
	during configuration time.  Recall that we are going to create the ``Socket``
	as a ``TcpSocket`` (which is implemented by ``TcpNewReno``) and hook 
	its "CongestionWindow" trace source before passing it to the ``Setup``
	method.

Neste código inicializamos os atributos da classe. Do ponto de vista do rastreamento, a mais importante é ``Ptr<Socket> socket`` que deve ser passado para a aplicação durante o fase de configuração. Lembre-se que vamos criar o ``Socket`` como um ``TcpSocket`` (que é implementado por ``TcpNewReno``) e associar sua origem do rastreamento de sua *"CongestionWindow"* antes de passá-lo no método ``Setup``.

::

  void
  MyApp::StartApplication (void)
  {
    m_running = true;
    m_packetsSent = 0;
    m_socket->Bind ();
    m_socket->Connect (m_peer);
    SendPacket ();
  }

..
	The above code is the overridden implementation ``Application::StartApplication``
	that will be automatically called by the simulator to start our ``Application``
	running at the appropriate time.  You can see that it does a ``Socket`` ``Bind``
	operation.  If you are familiar with Berkeley Sockets this shouldn't be a surprise.
	It performs the required work on the local side of the connection just as you might 
	expect.  The following ``Connect`` will do what is required to establish a connection 
	with the TCP at ``Address`` m_peer.  It should now be clear why we need to defer
	a lot of this to simulation time, since the ``Connect`` is going to need a fully
	functioning network to complete.  After the ``Connect``, the ``Application`` 
	then starts creating simulation events by calling ``SendPacket``.

Este código sobrescreve ``Application::StartApplication`` que será chamado automaticamente pelo simulador para iniciar a  ``Application`` no momento certo. Observamos que é realizada uma operação ``Socket`` ``Bind``. Se você conhece Sockets de Berkeley isto não é uma novidade. É responsável pelo conexão no lado do cliente, ou seja, o ``Connect`` estabelece uma  conexão usando TCP no endereço ``m_peer``. Por isso, precisamos de uma infraestrutura funcional de rede antes de executar a fase de simulação. Depois do ``Connect``, a ``Application`` inicia a criação dos eventos de simulação chamando ``SendPacket``.

::

  void
  MyApp::StopApplication (void)
  {
    m_running = false;
  
    if (m_sendEvent.IsRunning ())
      {
        Simulator::Cancel (m_sendEvent);
      }
  
    if (m_socket)
      {
        m_socket->Close ();
      }
  }

..
	Every time a simulation event is scheduled, an ``Event`` is created.  If the 
	``Event`` is pending execution or executing, its method ``IsRunning`` will
	return ``true``.  In this code, if ``IsRunning()`` returns true, we 
	``Cancel`` the event which removes it from the simulator event queue.  By 
	doing this, we break the chain of events that the ``Application`` is using to
	keep sending its ``Packets`` and the ``Application`` goes quiet.  After we 
	quiet the ``Application`` we ``Close`` the socket which tears down the TCP 
	connection.

A todo instante um evento da simulação é agendado, isto é, um ``Event`` é criado. Se o ``Event`` é uma execução pendente ou está executando, seu método ``IsRunning`` retornará ``true``. Neste código, se ``IsRunning()`` retorna verdadeiro (`true`), nós cancelamos (``Cancel``) o evento, e por consequência, é removido da fila de eventos do simulador. Dessa forma, interrompemos a cadeia de eventos que a 
``Application`` está usando para enviar seus ``Packets``. A Aplicação não enviará mais pacotes e em seguida fechamos (``Close``) o `socket` encerrando a conexão TCP.

..
	The socket is actually deleted in the destructor when the ``m_socket = 0`` is
	executed.  This removes the last reference to the underlying Ptr<Socket> which 
	causes the destructor of that Object to be called.

O socket é deletado no destrutor quando ``m_socket = 0`` é executado. Isto remove a última referência para Ptr<Socket>  que ocasiona o destrutor daquele Objeto ser chamado.

..
	Recall that ``StartApplication`` called ``SendPacket`` to start the 
	chain of events that describes the ``Application`` behavior.

Lembre-se que ``StartApplication`` chamou ``SendPacket`` para iniciar a cadeia de eventos que descreve o comportamento da ``Application``.

::

  void
  MyApp::SendPacket (void)
  {
    Ptr<Packet> packet = Create<Packet> (m_packetSize);
    m_socket->Send (packet);
  
    if (++m_packetsSent < m_nPackets)
      {
        ScheduleTx ();
      }
  }

..
	Here, you see that ``SendPacket`` does just that.  It creates a ``Packet``
	and then does a ``Send`` which, if you know Berkeley Sockets, is probably 
	just what you expected to see.

Este código apenas cria um pacote (``Packet``) e então envia (``Send``).

..
	It is the responsibility of the ``Application`` to keep scheduling the 
	chain of events, so the next lines call ``ScheduleTx`` to schedule another
	transmit event (a ``SendPacket``) until the ``Application`` decides it
	has sent enough.

É responsabilidade da ``Application`` gerenciar o agendamento da cadeia de eventos, então, a chamada ``ScheduleTx`` agenda outro evento de transmissão (um ``SendPacket``) até que a ``Application`` decida que enviou o suficiente.

::

  void
  MyApp::ScheduleTx (void)
  {
    if (m_running)
      {
        Time tNext (Seconds (m_packetSize * 8 / static_cast<double> (m_dataRate.GetBitRate ())));
        m_sendEvent = Simulator::Schedule (tNext, &MyApp::SendPacket, this);
      }
  }

..
	Here, you see that ``ScheduleTx`` does exactly that.  If the ``Application``
	is running (if ``StopApplication`` has not been called) it will schedule a 
	new event, which calls ``SendPacket`` again.  The alert reader will spot
	something that also trips up new users.  The data rate of an ``Application`` is
	just that.  It has nothing to do with the data rate of an underlying ``Channel``.
	This is the rate at which the ``Application`` produces bits.  It does not take
	into account any overhead for the various protocols or channels that it uses to 
	transport the data.  If you set the data rate of an ``Application`` to the same
	data rate as your underlying ``Channel`` you will eventually get a buffer overflow.

Enquanto a ``Application`` está executando, ``ScheduleTx`` agendará um novo evento, que chama ``SendPacket`` novamente. Verifica-se que a taxa de transmissão é sempre a mesma, ou seja, é a taxa que a ``Application`` produz os bits. Não considera nenhuma sobrecarga de protocolos ou canais físicos no transporte dos dados. Se alterarmos a taxa de transmissão da ``Application`` para a mesma taxa dos canais físicos,  poderemos
ter um estouro de *buffer*.

.. 
	The Trace Sinks

Destino do Rastreamento
~~~~~~~~~~~~~~~~~~~~~~~

..
	The whole point of this exercise is to get trace callbacks from TCP indicating the
	congestion window has been updated.  The next piece of code implements the 
	corresponding trace sink:

O foco deste exercício é obter notificações (*callbacks*) do TCP indicando a modificação da janela de congestionamento. O código a seguir implementa o destino do rastreamento.

::

  static void
  CwndChange (uint32_t oldCwnd, uint32_t newCwnd)
  {
    NS_LOG_UNCOND (Simulator::Now ().GetSeconds () << "\t" << newCwnd);
  }

..
	This should be very familiar to you now, so we won't dwell on the details.  This
	function just logs the current simulation time and the new value of the 
	congestion window every time it is changed.  You can probably imagine that you
	could load the resulting output into a graphics program (gnuplot or Excel) and
	immediately see a nice graph of the congestion window behavior over time.

Esta função registra o tempo de simulação atual e o novo valor da janela de congestionamento toda vez que é modificada. Poderíamos usar essa saída para construir um gráfico  do comportamento da janela de congestionamento com relação ao tempo.

..
	We added a new trace sink to show where packets are dropped.  We are going to 
	add an error model to this code also, so we wanted to demonstrate this working.

Nós adicionamos um novo destino do rastreamento para mostrar onde pacotes são perdidos. Vamos adicionar um modelo de erro.

::

  static void
  RxDrop (Ptr<const Packet> p)
  {
    NS_LOG_UNCOND ("RxDrop at " << Simulator::Now ().GetSeconds ());
  }

..
	This trace sink will be connected to the "PhyRxDrop" trace source of the 
	point-to-point NetDevice.  This trace source fires when a packet is dropped
	by the physical layer of a ``NetDevice``.  If you take a small detour to the
	source (``src/point-to-point/model/point-to-point-net-device.cc``) you will
	see that this trace source refers to ``PointToPointNetDevice::m_phyRxDropTrace``.
	If you then look in ``src/point-to-point/model/point-to-point-net-device.h``
	for this member variable, you will find that it is declared as a
	``TracedCallback<Ptr<const Packet> >``.  This should tell you that the
	callback target should be a function that returns void and takes a single
	parameter which is a ``Ptr<const Packet>`` -- just what we have above.

Este destino do rastreamento será conectado a origem do rastreamento "PhyRxDrop" do ``NetDevice`` ponto-a-ponto. Esta origem do rastreamento dispara quando um pacote é removido da camada física de um ``NetDevice``. Se olharmos rapidamente ``src/point-to-point/model/point-to-point-net-device.cc`` verificamos que a origem do rastreamento refere-se a ``PointToPointNetDevice::m_phyRxDropTrace``. E se procurarmos em ``src/point-to-point/model/point-to-point-net-device.h`` por essa variável, encontraremos que ela está declarada como uma ``TracedCallback<Ptr<const Packet> >``. Isto significa que nosso *callback* deve ser uma função que retorna ``void`` e tem um único parâmetro ``Ptr<const Packet>``.


.. 
	The Main Program

O Programa Principal
~~~~~~~~~~~~~~~~~~~~

.. 
	The following code should be very familiar to you by now:

O código a seguir corresponde ao início da função principal:

::

  int
  main (int argc, char *argv[])
  {
    NodeContainer nodes;
    nodes.Create (2);
  
    PointToPointHelper pointToPoint;
    pointToPoint.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
    pointToPoint.SetChannelAttribute ("Delay", StringValue ("2ms"));
  
    NetDeviceContainer devices;
    devices = pointToPoint.Install (nodes);

..
	This creates two nodes with a point-to-point channel between them, just as
	shown in the illustration at the start of the file.

São criados dois nós ligados por um canal ponto-a-ponto, como mostrado na ilustração
no início do arquivo.

..
	The next few lines of code show something new.  If we trace a connection that
	behaves perfectly, we will end up with a monotonically increasing congestion
	window.  To see any interesting behavior, we really want to introduce link 
	errors which will drop packets, cause duplicate ACKs and trigger the more
	interesting behaviors of the congestion window.


Nas próximas linhas, temos um código com algumas informações novas. Se nós 

rastrearmos uma conexão que comporta-se perfeitamente, terminamos com um

janela de congestionamento que aumenta monoliticamente. Para observarmos um

comportamento interessante, introduzimos erros que causarão perda de pacotes,

duplicação de ACK's e assim, introduz comportamentos mais interessantes a

janela de congestionamento.


..
	|ns3| provides ``ErrorModel`` objects which can be attached to
	``Channels``.  We are using the ``RateErrorModel`` which allows us
	to introduce errors into a ``Channel`` at a given *rate*. 


O |ns3| provê objetos de um modelo de erros (``ErrorModel``) que pode ser adicionado aos canais (``Channels``). Nós usamos o ``RateErrorModel`` que permite introduzir erros no canal dada uma *taxa*.

::

  Ptr<RateErrorModel> em = CreateObjectWithAttributes<RateErrorModel> (
    "RanVar", RandomVariableValue (UniformVariable (0., 1.)),
    "ErrorRate", DoubleValue (0.00001));
  devices.Get (1)->SetAttribute ("ReceiveErrorModel", PointerValue (em));

..
	The above code instantiates a ``RateErrorModel`` Object.  Rather than 
	using the two-step process of instantiating it and then setting Attributes,
	we use the convenience function ``CreateObjectWithAttributes`` which
	allows us to do both at the same time.  We set the "RanVar" 
	``Attribute`` to a random variable that generates a uniform distribution
	from 0 to 1.  We also set the "ErrorRate" ``Attribute``.
	We then set the resulting instantiated ``RateErrorModel`` as the error
	model used by the point-to-point ``NetDevice``.  This will give us some
	retransmissions and make our plot a little more interesting.

O código instancia um objeto ``RateErrorModel``. Para simplificar usamos a função ``CreateObjectWithAttributes`` que instancia e configura os Atributos. O Atributo "RanVar" foi configurado para uma variável randômica que gera uma distribuição uniforme entre 0 e 1. O Atributo "ErrorRate" também foi alterado. Por fim, configuramos o modelo erro no ``NetDevice`` ponto-a-ponto modificando o atributo "ReceiveErrorModel".  Isto causará retransmissões e o gráfico ficará mais interessante.

::

  InternetStackHelper stack;
  stack.Install (nodes);

  Ipv4AddressHelper address;
  address.SetBase ("10.1.1.0", "255.255.255.252");
  Ipv4InterfaceContainer interfaces = address.Assign (devices);

..
	The above code should be familiar.  It installs internet stacks on our two
	nodes and creates interfaces and assigns IP addresses for the point-to-point
	devices.

Neste código configura a pilha de protocolos da internet nos dois nós de rede, cria interfaces e associa endereços IP para os dispositivos ponto-a-ponto.

..
	Since we are using TCP, we need something on the destination node to receive
	TCP connections and data.  The ``PacketSink`` ``Application`` is commonly
	used in |ns3| for that purpose.

Como estamos usando TCP, precisamos de um nó de destino para receber as conexões e os dados.  O ``PacketSink`` ``Application``  é comumente usado no |ns3| para este propósito.

::

  uint16_t sinkPort = 8080;
  Address sinkAddress (InetSocketAddress(interfaces.GetAddress (1), sinkPort));
  PacketSinkHelper packetSinkHelper ("ns3::TcpSocketFactory", 
    InetSocketAddress (Ipv4Address::GetAny (), sinkPort));
  ApplicationContainer sinkApps = packetSinkHelper.Install (nodes.Get (1));
  sinkApps.Start (Seconds (0.));
  sinkApps.Stop (Seconds (20.));

.. 
	This should all be familiar, with the exception of,

Este código deveria ser familiar, com exceção de,

::

  PacketSinkHelper packetSinkHelper ("ns3::TcpSocketFactory", 
    InetSocketAddress (Ipv4Address::GetAny (), sinkPort));

..
	This code instantiates a ``PacketSinkHelper`` and tells it to create sockets
	using the class ``ns3::TcpSocketFactory``.  This class implements a design 
	pattern called "object factory" which is a commonly used mechanism for 
	specifying a class used to create objects in an abstract way.  Here, instead of 
	having to create the objects themselves, you provide the ``PacketSinkHelper``
	a string that specifies a ``TypeId`` string used to create an object which 
	can then be used, in turn, to create instances of the Objects created by the 
	factory.

Este código instancia um ``PacketSinkHelper`` e cria sockets usando a classe ``ns3::TcpSocketFactory``. Esta classe implementa o padrão de projeto "fábrica de objetos". Dessa forma, em vez de criar os objetos diretamente, fornecemos ao ``PacketSinkHelper`` um texto que especifica um ``TypeId`` usado para criar
um objeto que, por sua vez, pode ser usado para criar instâncias de Objetos criados pela implementação da fábrica de objetos.

..
	The remaining parameter tells the ``Application`` which address and port it
	should ``Bind`` to.

O parâmetro seguinte especifica o endereço e a porta para o mapeamento.

.. 
	The next two lines of code will create the socket and connect the trace source.

As próximas duas linhas do código criam o `socket` e conectam a origem do rastreamento.

::

  Ptr<Socket> ns3TcpSocket = Socket::CreateSocket (nodes.Get (0), 
    TcpSocketFactory::GetTypeId ());
  ns3TcpSocket->TraceConnectWithoutContext ("CongestionWindow", 
    MakeCallback (&CwndChange));

..
	The first statement calls the static member function ``Socket::CreateSocket``
	and provides a ``Node`` and an explicit ``TypeId`` for the object factory
	used to create the socket.  This is a slightly lower level call than the 
	``PacketSinkHelper`` call above, and uses an explicit C++ type instead of 
	one referred to by a string.  Otherwise, it is conceptually the same thing.

A primeira declaração chama a função estática ``Socket::CreateSocket`` e passa um ``Node`` e um ``TypeId`` para o objeto fábrica usado para criar o `socket`. 

..
	Once the ``TcpSocket`` is created and attached to the ``Node``, we can
	use ``TraceConnectWithoutContext`` to connect the CongestionWindow trace 
	source to our trace sink.

Uma vez que o ``TcpSocket`` é criado e adicionado ao ``Node``, nós usamos ``TraceConnectWithoutContext`` para conectar a origem do rastreamento "CongestionWindow" para o nosso destino do rastreamento.

..
	Recall that we coded an ``Application`` so we could take that ``Socket``
	we just made (during configuration time) and use it in simulation time.  We now 
	have to instantiate that ``Application``.  We didn't go to any trouble to
	create a helper to manage the ``Application`` so we are going to have to 
	create and install it "manually".  This is actually quite easy:

Codificamos uma ``Application`` então podemos obter um ``Socket`` (durante a fase de configuração) e usar na fase de simulação. Temos agora que instanciar a ``Application``. Para tal, segue os passos:

::

  Ptr<MyApp> app = CreateObject<MyApp> ();
  app->Setup (ns3TcpSocket, sinkAddress, 1040, 1000, DataRate ("1Mbps"));
  nodes.Get (0)->AddApplication (app);
  app->Start (Seconds (1.));
  app->Stop (Seconds (20.));

..
	The first line creates an ``Object`` of type ``MyApp`` -- our
	``Application``.  The second line tells the ``Application`` what
	``Socket`` to use, what address to connect to, how much data to send 
	at each send event, how many send events to generate and the rate at which
	to produce data from those events.

A primeira linha cria um Objeto do tipo ``MyApp`` -- nossa ``Application``. A segunda linha especifica o `socket`, o endereço de conexão, a quantidade de dados a ser enviada em cada evento, a quantidade de eventos de transmissão a ser gerados e a taxa de produção de dados para estes eventos.

	Next, we manually add the ``MyApp Application`` to the source node
	and explicitly call the ``Start`` and ``Stop`` methods on the 
	``Application`` to tell it when to start and stop doing its thing.

Depois, adicionamos a ``MyApp Application`` para o nó origem e chamamos os métodos ``Start`` e ``Stop`` para dizer quando e iniciar e parar a simulação.

..
	We need to actually do the connect from the receiver point-to-point ``NetDevice``
	to our callback now.

Precisamos agora fazer a conexão entre o receptor com nossa *callback*.

::

  devices.Get (1)->TraceConnectWithoutContext("PhyRxDrop", MakeCallback (&RxDrop));

..
	It should now be obvious that we are getting a reference to the receiving 
	``Node NetDevice`` from its container and connecting the trace source defined
	by the attribute "PhyRxDrop" on that device to the trace sink ``RxDrop``.

Estamos obtendo uma referência para o ``Node NetDevice`` receptor e conectando a origem do rastreamento pelo Atributo "PhyRxDrop" do dispositivo no destino do rastreamento ``RxDrop``.

..
	Finally, we tell the simulator to override any ``Applications`` and just
	stop processing events at 20 seconds into the simulation.

Finalmente, dizemos ao simulador para sobrescrever qualquer ``Applications`` e parar o processamento de eventos em 20 segundos na simulação.

::

    Simulator::Stop (Seconds(20));
    Simulator::Run ();
    Simulator::Destroy ();

    return 0;
  }

..
	Recall that as soon as ``Simulator::Run`` is called, configuration time
	ends, and simulation time begins.  All of the work we orchestrated by 
	creating the ``Application`` and teaching it how to connect and send
	data actually happens during this function call.

Lembre-se que quando ``Simulator::Run`` é chamado, a fase de configuração termina e a fase de simulação inicia. Todo o processo descrito anteriormente ocorre durante a chamada dessa função.

..
	As soon as ``Simulator::Run`` returns, the simulation is complete and
	we enter the teardown phase.  In this case, ``Simulator::Destroy`` takes
	care of the gory details and we just return a success code after it completes.

Após o retorno do ``Simulator::Run``, a simulação é terminada e entramos na fase de finalização. Neste caso, ``Simulator::Destroy`` executa a tarefa pesada e nós apenas retornamos o código de sucesso.

.. 
	Running fifth.cc

Executando fifth.cc
+++++++++++++++++++

..
	Since we have provided the file ``fifth.cc`` for you, if you have built
	your distribution (in debug mode since it uses NS_LOG -- recall that optimized
	builds optimize out NS_LOGs) it will be waiting for you to run.

O arquivo ``fifth.cc`` é distribuído no código fonte, no diretório ``examples/tutorial``. Para executar:

::

  ./waf --run fifth
  Waf: Entering directory `/home/craigdo/repos/ns-3-allinone-dev/ns-3-dev/build
  Waf: Leaving directory `/home/craigdo/repos/ns-3-allinone-dev/ns-3-dev/build'
  'build' finished successfully (0.684s)
  1.20919 1072
  1.21511 1608
  1.22103 2144
  ...
  1.2471  8040
  1.24895 8576
  1.2508  9112
  RxDrop at 1.25151
  ...

..
	You can probably see immediately a downside of using prints of any kind in your
	traces.  We get those extraneous waf messages printed all over our interesting
	information along with those RxDrop messages.  We will remedy that soon, but I'm
	sure you can't wait to see the results of all of this work.  Let's redirect that
	output to a file called ``cwnd.dat``:

Podemos observar o lado negativo de usar "prints" de qualquer tipo no rastreamento. Temos mensagens ``waf`` sendo impressas sobre a informação relevante. Vamos resolver esse problema, mas primeiro vamos verificar o resultado redirecionando a saída para um arquivo ``cwnd.dat``:

::

  ./waf --run fifth > cwnd.dat 2>&1

..
	Now edit up "cwnd.dat" in your favorite editor and remove the waf build status
	and drop lines, leaving only the traced data (you could also comment out the
	``TraceConnectWithoutContext("PhyRxDrop", MakeCallback (&RxDrop));`` in the
	script to get rid of the drop prints just as easily. 

Removemos as mensagens do ``waf`` e deixamos somente os dados rastreados.  Pode-se também comentar as mensagens de "RxDrop...".

..
	You can now run gnuplot (if you have it installed) and tell it to generate some 
	pretty pictures:

Agora podemos executar o gnuplot (se instalado) e gerar um gráfico:

::

  gnuplot> set terminal png size 640,480
  gnuplot> set output "cwnd.png"
  gnuplot> plot "cwnd.dat" using 1:2 title 'Congestion Window' with linespoints
  gnuplot> exit

..
	You should now have a graph of the congestion window versus time sitting in the 
	file "cwnd.png" that looks like:

Devemos obter um gráfico da janela de congestionamento pelo tempo no arquivo "cwnd.png", similar ao gráfico 7.1:

figure:: figures/cwnd.png

   Gráfico da janela de congestionamento versus tempo.

.. 
	Using Mid-Level Helpers

Usando Auxiliares Intermediários
++++++++++++++++++++++++++++++++

..
	In the previous section, we showed how to hook a trace source and get hopefully
	interesting information out of a simulation.  Perhaps you will recall that we 
	called logging to the standard output using ``std::cout`` a "Blunt Instrument" 
	much earlier in this chapter.  We also wrote about how it was a problem having
	to parse the log output in order to isolate interesting information.  It may 
	have occurred to you that we just spent a lot of time implementing an example
	that exhibits all of the problems we purport to fix with the |ns3| tracing
	system!  You would be correct.  But, bear with us.  We're not done yet.

Na seção anterior, mostramos como adicionar uma origem do rastreamento e obter informações de interesse fora da simulação. Entretanto, no início do capítulo foi comentado que imprimir informações na saída padrão não é uma boa prática. Além disso, comentamos que não é interessante realizar processamento sobre a saída para isolar a informação de interesse. Podemos pensar que perdemos muito tempo em um exemplo que apresenta todos os problemas que propomos resolver usando o sistema de rastreamento do |ns3|. Você estaria correto, mas nós ainda não terminamos.

..
	One of the most important things we want to do is to is to have the ability to 
	easily control the amount of output coming out of the simulation; and we also 
	want to save those data to a file so we can refer back to it later.  We can use
	the mid-level trace helpers provided in |ns3| to do just that and complete
	the picture.

Uma da coisas mais importantes que queremos fazer é controlar a quantidade de saída da simulação. Nós podemos usar assistentes de rastreamento intermediários fornecido pelo |ns3| para alcançar com sucesso esse objetivo.

..
	We provide a script that writes the cwnd change and drop events developed in 
	the example ``fifth.cc`` to disk in separate files.  The cwnd changes are 
	stored as a tab-separated ASCII file and the drop events are stored in a pcap
	file.  The changes to make this happen are quite small.

Fornecemos um código que separa em arquivos distintos no disco os eventos de modificação da janela e os eventos de remoção. As alterações em cwnd são armazenadas em um arquivo ASCII separadas por TAB e os eventos de remoção são armazenados em um arquivo *pcap*. As alterações para obter esse resultado são pequenas.

.. 
	A sixth.cc Walkthrough

Analisando sixth.cc
~~~~~~~~~~~~~~~~~~~

..
	Let's take a look at the changes required to go from ``fifth.cc`` to 
	``sixth.cc``.  Open ``examples/tutorial/fifth.cc`` in your favorite 
	editor.  You can see the first change by searching for CwndChange.  You will 
	find that we have changed the signatures for the trace sinks and have added 
	a single line to each sink that writes the traced information to a stream
	representing a file.

Vamos verificar as mudanças do arquivo ``fifth.cc`` para o  ``sixth.cc``. Verificamos a primeira mudança em ``CwndChange``. Notamos que as assinaturas para o destino do rastreamento foram alteradas e que foi adicionada uma linha para cada um que escreve a informação rastreada para um fluxo (*stream*) representando um arquivo

::

  static void
  CwndChange (Ptr<OutputStreamWrapper> stream, uint32_t oldCwnd, uint32_t newCwnd)
  {
    NS_LOG_UNCOND (Simulator::Now ().GetSeconds () << "\t" << newCwnd);
    *stream->GetStream () << Simulator::Now ().GetSeconds () << "\t" 
    		<< oldCwnd << "\t" << newCwnd << std::endl;
  }
  
  static void
  RxDrop (Ptr<PcapFileWrapper> file, Ptr<const Packet> p)
  {
    NS_LOG_UNCOND ("RxDrop at " << Simulator::Now ().GetSeconds ());
    file->Write(Simulator::Now(), p);
  }

..
	We have added a "stream" parameter to the ``CwndChange`` trace sink.  
	This is an object that holds (keeps safely alive) a C++ output stream.  It 
	turns out that this is a very simple object, but one that manages lifetime 
	issues for the stream and solves a problem that even experienced C++ users 
	run into.  It turns out that the copy constructor for ostream is marked 
	private.  This means that ostreams do not obey value semantics and cannot 
	be used in any mechanism that requires the stream to be copied.  This includes
	the |ns3| callback system, which as you may recall, requires objects
	that obey value semantics.  Further notice that we have added the following 
	line in the ``CwndChange`` trace sink implementation:

Um parâmetro "stream" foi adicionado para o destino do rastreamento ``CwndChange``. Este é um objeto que armazena (mantém seguramente vivo) um fluxo de saída em C++. Isto resulta em um objeto muito simples, mas que gerência problemas no ciclo de vida para fluxos e resolve um problema que mesmo programadores experientes de C++ tem dificuldades. Resulta que o construtor de cópia para o fluxo de saída (*ostream*) é marcado como privado. Isto significa que fluxos de saída não seguem a semântica de passagem por valor e não podem ser usados em mecanismos que necessitam que o fluxo seja copiado. Isto inclui o sistema de *callback* do |ns3|. Além disso, adicionamos a seguinte linha:

::

  *stream->GetStream () << Simulator::Now ().GetSeconds () << "\t" << oldCwnd 
  		<< "\t" << newCwnd << std::endl;

..
	This would be very familiar code if you replaced ``*stream->GetStream ()``
	with ``std::cout``, as in:

que substitui ``std::cout`` por ``*stream->GetStream ()``

::

  std::cout << Simulator::Now ().GetSeconds () << "\t" << oldCwnd << "\t" << 
  		newCwnd << std::endl;

..
	This illustrates that the ``Ptr<OutputStreamWrapper>`` is really just
	carrying around a ``std::ofstream`` for you, and you can use it here like 
	any other output stream.

Isto demostra que o ``Ptr<OutputStreamWrapper>`` está apenas encapsulando um ``std::ofstream``, logo pode ser usado como qualquer outro fluxo de saída.

..
	A similar situation happens in ``RxDrop`` except that the object being 
	passed around (a ``Ptr<PcapFileWrapper>``) represents a pcap file.  There
	is a one-liner in the trace sink to write a timestamp and the contents of the 
	packet being dropped to the pcap file:

Uma situação similar ocorre em ``RxDrop``, exceto que o objeto passado (``Ptr<PcapFileWrapper>``) representa um arquivo pcap. Há uma linha no *trace sink* para escrever um marcador de tempo (*timestamp*) eo conteúdo do pacote perdido para o arquivo pcap.

::

  file->Write(Simulator::Now(), p);

..
	Of course, if we have objects representing the two files, we need to create
	them somewhere and also cause them to be passed to the trace sinks.  If you 
	look in the ``main`` function, you will find new code to do just that:

É claro, se nós temos objetos representando os dois arquivos, precisamos criá-los em algum lugar e também passá-los aos *trace sinks*. Se observarmos a função ``main``, temos o código:

::

  AsciiTraceHelper asciiTraceHelper;
  Ptr<OutputStreamWrapper> stream = asciiTraceHelper.CreateFileStream ("sixth.cwnd");
  ns3TcpSocket->TraceConnectWithoutContext ("CongestionWindow", 
  		MakeBoundCallback (&CwndChange, stream));

  ...

  PcapHelper pcapHelper;
  Ptr<PcapFileWrapper> file = pcapHelper.CreateFile ("sixth.pcap", 
  		std::ios::out, PcapHelper::DLT_PPP);
  devices.Get (1)->TraceConnectWithoutContext("PhyRxDrop", 
  		MakeBoundCallback (&RxDrop, file));

..
	In the first section of the code snippet above, we are creating the ASCII
	trace file, creating an object responsible for managing it and using a
	variant of the callback creation function to arrange for the object to be 
	passed to the sink.  Our ASCII trace helpers provide a rich set of
	functions to make using text (ASCII) files easy.  We are just going to 
	illustrate the use of the file stream creation function here.

Na primeira seção do código, criamos o arquivo de rastreamento ASCII e o objeto responsável para gerenciá-lo. Em seguida, usando uma das formas da função para criação da *callback* permitimos o objeto ser passado para o destino do rastreamento. As classes assistentes para rastreamento ASCII fornecem um vasto conjunto de funções para facilitar a manipulação de arquivos texto. Neste exemplo, focamos apenas na criação do arquivo para o fluxo de saída.

..
	The ``CreateFileStream{}`` function is basically going to instantiate
	a std::ofstream object and create a new file (or truncate an existing file).
	This ofstream is packaged up in an |ns3| object for lifetime management
	and copy constructor issue resolution.

A função ``CreateFileStream()`` instancia um objeto ``std::ofstream`` e cria  um novo arquivo. O fluxo de saída ``ofstream`` é encapsulado em um objeto do |ns3| para gerenciamento do ciclo de vida e para resolver o
problema do construtor de cópia.

..
	We then take this |ns3| object representing the file and pass it to
	``MakeBoundCallback()``.  This function creates a callback just like
	``MakeCallback()``, but it "binds" a new value to the callback.  This
	value is added to the callback before it is called.

Então pegamos o objeto que representa o arquivo e passamos para ``MakeBoundCallback()``. Esta função cria um *callback* como ``MakeCallback()``, mas "associa" um novo valor para o *callback*. Este valor é adicionado ao *callback* antes de sua invocação.

..
	Essentially, ``MakeBoundCallback(&CwndChange, stream)`` causes the trace 
	source to add the additional "stream" parameter to the front of the formal
	parameter list before invoking the callback.  This changes the required 
	signature of the ``CwndChange`` sink to match the one shown above, which
	includes the "extra" parameter ``Ptr<OutputStreamWrapper> stream``.

Essencialmente, ``MakeBoundCallback(&CwndChange, stream)`` faz com que a origem do rastreamento adicione um parâmetro extra "fluxo" após a lista formal de parâmetros antes de invocar o *callback*. Esta mudança está de acordo com o apresentado anteriormente, a qual inclui o parâmetro ``Ptr<OutputStreamWrapper> stream``.

..
	In the second section of code in the snippet above, we instantiate a 
	``PcapHelper`` to do the same thing for our pcap trace file that we did
	with the ``AsciiTraceHelper``. The line of code,

Na segunda seção de código, instanciamos um ``PcapHelper`` para fazer a mesma coisa para o arquivo de rastreamento pcap. A linha de código,

::

  Ptr<PcapFileWrapper> file = pcapHelper.CreateFile ("sixth.pcap", "w", 
  		PcapHelper::DLT_PPP);

..
	creates a pcap file named "sixth.pcap" with file mode "w".   This means that
	the new file is to truncated if an existing file with that name is found.  The
	final parameter is the "data link type" of the new pcap file.  These are 
	the same as the pcap library data link types defined in ``bpf.h`` if you are
	familar with pcap.  In this case, ``DLT_PPP`` indicates that the pcap file
	is going to contain packets prefixed with point to point headers.  This is true
	since the packets are coming from our point-to-point device driver.  Other
	common data link types are DLT_EN10MB (10 MB Ethernet) appropriate for csma
	devices and DLT_IEEE802_11 (IEEE 802.11) appropriate for wifi devices.  These
	are defined in ``src/network/helper/trace-helper.h"`` if you are interested in seeing
	the list.  The entries in the list match those in ``bpf.h`` but we duplicate
	them to avoid a pcap source dependence.

cria um arquivo pcap chamado "sixth.pcap" no modo "w" (escrita). O parâmetro final é o "tipo da ligação de dados" do arquivo pcap. As opções estão definidas em ``bpf.h``. Neste caso, ``DLT_PPP`` indica que o arquivo pcap deverá conter pacotes prefixado com cabeçalhos ponto-a-ponto. Isto é verdade pois os pacotes estão chegando de nosso `driver` de dispositivo ponto-a-ponto. Outros tipos de ligação de dados comuns são DLT_EN10MB (10 MB Ethernet) apropriado para dispositivos CSMA e DLT_IEEE802_11 (IEEE 802.11) apropriado para dispositivos sem fio. O arquivo ``src/network/helper/trace-helper.h"`` define uma lista com os tipos. As entradas na lista são idênticas as definidas em ``bpf.h``, pois foram duplicadas para evitar um dependência com o pcap.

..
	A |ns3| object representing the pcap file is returned from ``CreateFile``
	and used in a bound callback exactly as it was in the ascii case.

Um objeto |ns3| representando o arquivo pcap é retornado de ``CreateFile`` e usado em uma *callback* exatamente como no caso ASCII.

..
	An important detour:  It is important to notice that even though both of these 
	objects are declared in very similar ways,

É importante observar que ambos objetos são declarados de maneiras muito similares,

::

  Ptr<PcapFileWrapper> file ...
  Ptr<OutputStreamWrapper> stream ...

..
	The underlying objects are entirely different.  For example, the 
	Ptr<PcapFileWrapper> is a smart pointer to an |ns3| Object that is a 
	fairly heaviweight thing that supports ``Attributes`` and is integrated into
	the config system.  The Ptr<OutputStreamWrapper>, on the other hand, is a smart 
	pointer to a reference counted object that is a very lightweight thing.
	Remember to always look at the object you are referencing before making any
	assumptions about the "powers" that object may have.  

Mas os objetos internos são inteiramente diferentes. Por exemplo, o Ptr<PcapFileWrapper> é um ponteiro para um objeto |ns3| que suporta ``Attributes`` e é integrado dentro do sistema de configuração. O Ptr<OutputStreamWrapper>, por outro lado, é um ponteiro para uma referência para um simples objeto contado. Lembre-se sempre de analisar o objeto que você está referenciando antes de fazer suposições sobre os "poderes" que o objeto pode ter.

..
	For example, take a look at ``src/network/utils/pcap-file-wrapper.h`` in the 
	distribution and notice, 

Por exemplo, acesse o arquivo ``src/network/utils/pcap-file-wrapper.h`` e observe,

::

  class PcapFileWrapper : public Object

..
	that class ``PcapFileWrapper`` is an |ns3| Object by virtue of 
	its inheritance.  Then look at ``src/network/model/output-stream-wrapper.h`` and 
	notice,

que a classe ``PcapFileWrapper`` é um ``Object`` |ns3| por herança. Já no arquivo ``src/network/model/output-stream-wrapper.h``, observe,

::

  class OutputStreamWrapper : public SimpleRefCount<OutputStreamWrapper>

..
	that this object is not an |ns3| Object at all, it is "merely" a
	C++ object that happens to support intrusive reference counting.

que não é um ``Object`` |ns3|, mas um objeto C++ que suporta contagem de referência.

..
	The point here is that just because you read Ptr<something> it does not necessarily
	mean that "something" is an |ns3| Object on which you can hang |ns3|
	``Attributes``, for example.

A questão é que se você tem um Ptr<alguma_coisa>, não necessariamente significa que "alguma_coisa" é um ``Object`` |ns3|, no qual você pode modificar ``Attributes``, por exemplo.

..
	Now, back to the example.  If you now build and run this example,

Voltando ao exemplo. Se compilarmos e executarmos o exemplo,

::

  ./waf --run sixth

..
	you will see the same messages appear as when you ran "fifth", but two new 
	files will appear in the top-level directory of your |ns3| distribution.

Veremos as mesmas mensagens do "fifth", mas dois novos arquivos aparecerão no diretório base de sua distribuição do |ns3|.

::

  sixth.cwnd  sixth.pcap

..
	Since "sixth.cwnd" is an ASCII text file, you can view it with ``cat``
	or your favorite file viewer.

Como "sixth.cwnd" é um arquivo texto ASCII, você pode visualizar usando *cat* ou um editor de texto.

::

  1.20919 536     1072
  1.21511 1072    1608
  ...
  9.30922 8893    8925
  9.31754 8925    8957

..
	You have a tab separated file with a timestamp, an old congestion window and a
	new congestion window suitable for directly importing into your plot program.
	There are no extraneous prints in the file, no parsing or editing is required.
	
Cada linha tem um marcador de tempo, o valor da janela de congestionamento e o valor da nova janela de congestionamento separados por tabulação, para importar diretamente para seu programa de plotagem de gráficos.
Não há nenhuma outra informação além da rastreada, logo não é necessário processamento ou edição do arquivo.

..
	Since "sixth.pcap" is a pcap file, you can view it with ``tcpdump``.

Como "sixth.pcap" é um arquivo pcap, você pode visualizar usando o ``tcpdump`` ou ``wireshark``.

::

  reading from file ../../sixth.pcap, link-type PPP (PPP)
  1.251507 IP 10.1.1.1.49153 > 10.1.1.2.8080: . 17689:18225(536) ack 1 win 65535
  1.411478 IP 10.1.1.1.49153 > 10.1.1.2.8080: . 33808:34312(504) ack 1 win 65535
  ...
  7.393557 IP 10.1.1.1.49153 > 10.1.1.2.8080: . 781568:782072(504) ack 1 win 65535
  8.141483 IP 10.1.1.1.49153 > 10.1.1.2.8080: . 874632:875168(536) ack 1 win 65535

..
	You have a pcap file with the packets that were dropped in the simulation.  There
	are no other packets present in the file and there is nothing else present to
	make life difficult.

Você tem um arquivo pcap com os pacotes que foram descartados na simulação. Não há nenhum outro pacote presente no arquivo e nada mais para dificultar sua análise.

..
	It's been a long journey, but we are now at a point where we can appreciate the
	|ns3| tracing system.  We have pulled important events out of the middle
	of a TCP implementation and a device driver.  We stored those events directly in
	files usable with commonly known tools.  We did this without modifying any of the
	core code involved, and we did this in only 18 lines of code:

Foi uma longa jornada, mas agora entendemos porque o sistema de rastreamento é interessante. Nós obtemos e armazenamos importantes eventos da implementação do TCP e do `driver` de dispositivo. E não modificamos nenhuma linha do código do núcleo do |ns3|, e ainda fizemos isso com apenas 18 linhas de código:

::

  static void
  CwndChange (Ptr<OutputStreamWrapper> stream, uint32_t oldCwnd, uint32_t newCwnd)
  {
    NS_LOG_UNCOND (Simulator::Now ().GetSeconds () << "\t" << newCwnd);
    *stream->GetStream () << Simulator::Now ().GetSeconds () << "\t" << 
    		oldCwnd << "\t" << newCwnd << std::endl;
  }

  ...

  AsciiTraceHelper asciiTraceHelper;
  Ptr<OutputStreamWrapper> stream = asciiTraceHelper.CreateFileStream ("sixth.cwnd");
  ns3TcpSocket->TraceConnectWithoutContext ("CongestionWindow", 
  		MakeBoundCallback (&CwndChange, stream));

  ...

  static void
  RxDrop (Ptr<PcapFileWrapper> file, Ptr<const Packet> p)
  {
    NS_LOG_UNCOND ("RxDrop at " << Simulator::Now ().GetSeconds ());
    file->Write(Simulator::Now(), p);
  }

  ...
  
  PcapHelper pcapHelper;
  Ptr<PcapFileWrapper> file = pcapHelper.CreateFile ("sixth.pcap", "w", 
  		PcapHelper::DLT_PPP);
  devices.Get (1)->TraceConnectWithoutContext("PhyRxDrop", 
  		MakeBoundCallback (&RxDrop, file));

.. 
	Using Trace Helpers

Usando Classes Assistentes para Rastreamento
********************************************

..
	The |ns3| trace helpers provide a rich environment for configuring and
	selecting different trace events and writing them to files.  In previous
	sections, primarily "Building Topologies," we have seen several varieties
	of the trace helper methods designed for use inside other (device) helpers.

As classes assistentes (*trace helpers*) de rastreamento do |ns3| proveem um ambiente rico para configurar, selecionar e escrever diferentes eventos de rastreamento para arquivos. Nas seções anteriores, primeiramente em "Construindo Topologias", nós vimos diversas formas de métodos assistentes para rastreamento projetados para uso dentro de outras classes assistentes.

..
	Perhaps you will recall seeing some of these variations: 

Segue alguns desses métodos já estudados:

::

  pointToPoint.EnablePcapAll ("second");
  pointToPoint.EnablePcap ("second", p2pNodes.Get (0)->GetId (), 0);
  csma.EnablePcap ("third", csmaDevices.Get (0), true);
  pointToPoint.EnableAsciiAll (ascii.CreateFileStream ("myfirst.tr"));

..
	What may not be obvious, though, is that there is a consistent model for all of 
	the trace-related methods found in the system.  We will now take a little time
	and take a look at the "big picture".


O que não parece claro é que há um modelo consistente para todos os métodos relacionados à rastreamento encontrados no sistema. Apresentaremos uma visão geral desse modelo.

..
	There are currently two primary use cases of the tracing helpers in |ns3|:
	Device helpers and protocol helpers.  Device helpers look at the problem
	of specifying which traces should be enabled through a node, device pair.  For 
	example, you may want to specify that pcap tracing should be enabled on a 
	particular device on a specific node.  This follows from the |ns3| device
	conceptual model, and also the conceptual models of the various device helpers.
	Following naturally from this, the files created follow a 
	<prefix>-<node>-<device> naming convention.  

Há dois casos de uso primários de classes assistentes em |ns3|: Classes assistentes de dispositivo e classes assistentes de protocolo. Classes assistentes de dispositivo tratam o problema de especificar quais rastreamentos deveriam ser habilitados no domínio do nó de rede. Por exemplo, poderíamos querer especificar que o rastreamento pcap deveria ser ativado em um dispositivo particular de um nó específico. Isto é o que define o modelo conceitual de dispositivo no |ns3| e também os modelos conceituais de várias classes assistentes de dispositivos. Baseado nisso, os arquivos criados seguem a convenção de nome `<prefixo>-<nó>-<dispositivo>`.

..
	Protocol helpers look at the problem of specifying which traces should be
	enabled through a protocol and interface pair.  This follows from the |ns3|
	protocol stack conceptual model, and also the conceptual models of internet
	stack helpers.  Naturally, the trace files should follow a 
	<prefix>-<protocol>-<interface> naming convention.

As classes assistentes de protocolos tratam o problema de especificar quais rastreamentos deveriam ser ativados no protocolo e interface. Isto é definido pelo modelo conceitual de pilha de protocolo do |ns3| e também pelos modelos conceituais de classes assistentes de pilha de rede. Baseado nisso, os arquivos criados seguem a convenção de nome `<prefixo>-<protocolo>-<interface>`.

..
	The trace helpers therefore fall naturally into a two-dimensional taxonomy.
	There are subtleties that prevent all four classes from behaving identically,
	but we do strive to make them all work as similarly as possible; and whenever
	possible there are analogs for all methods in all classes.

As classes assistentes consequentemente encaixam-se em uma taxinomia bi-dimensional. Há pequenos detalhes que evitam todas as classes comportarem-se da mesma forma, mas fizemos parecer que trabalham tão similarmente quanto possível e quase sempre há similares para todos métodos em todas as classes.

::

                                                     | pcap | ascii |
  ---------------------------------------------------+------+-------|
  Classe Assistente de Dispositivo (*Device Helper*)   |      |       |
  ---------------------------------------------------+------+-------|
  Classe Assistente de Protocolo (*Protocol Helper*)   |      |       |
  ---------------------------------------------------+------+-------|

..
	We use an approach called a ``mixin`` to add tracing functionality to our 
	helper classes.  A ``mixin`` is a class that provides functionality to that
	is inherited by a subclass.  Inheriting from a mixin is not considered a form 
	of specialization but is really a way to collect functionality. 

Usamos uma abordagem chamada ``mixin`` para adicionar funcionalidade de rastreamento para nossas classes assistentes. Uma ``mixin`` é uma classe que provê funcionalidade para aquela que é herdada por uma subclasse. Herdar de um ``mixin`` não é considerado uma forma de especialização mas é realmente uma maneira de colecionar funcionalidade.

..
	Let's take a quick look at all four of these cases and their respective 
	``mixins``.

Vamos verificar rapidamente os quatro casos e seus respectivos ``mixins``.

.. 
	Pcap Tracing Device Helpers

Classes Assistentes de Dispositivo para Rastreamento Pcap
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

..
	The goal of these helpers is to make it easy to add a consistent pcap trace
	facility to an |ns3| device.  We want all of the various flavors of
	pcap tracing to work the same across all devices, so the methods of these 
	helpers are inherited by device helpers.  Take a look at 
	``src/network/helper/trace-helper.h`` if you want to follow the discussion while 
	looking at real code.

O objetivo destes assistentes é simplificar a adição de um utilitário de rastreamento pcap consistente para um dispositivo |ns3|. Queremos que opere da mesma forma entre todos os dispositivos, logo os métodos destes assistentes são herdados por classes assistentes de dispositivo. Observe o arquivo ``src/network/helper/trace-helper.h`` para entender a discussão do código a seguir.

..
	The class ``PcapHelperForDevice`` is a ``mixin`` provides the high level 
	functionality for using pcap tracing in an |ns3| device.  Every device 
	must implement a single virtual method inherited from this class.

A classe ``PcapHelperForDevice`` é um ``mixin`` que provê a funcionalidade de alto nível para usar rastreamento pcap em um dispositivo |ns3|. Todo dispositivo deve implementar um único método virtual herdado dessa classe.

::

  virtual void EnablePcapInternal (std::string prefix, Ptr<NetDevice> nd, 
  		bool promiscuous, bool explicitFilename) = 0;

..
	The signature of this method reflects the device-centric view of the situation
	at this level.  All of the public methods inherited from class 
	``PcapUserHelperForDevice`` reduce to calling this single device-dependent
	implementation method.  For example, the lowest level pcap method,

A assinatura deste método reflete a visão do dispositivo da situação neste nível. Todos os métodos públicos herdados da classe ``PcapUserHelperForDevice`` são reduzidos a chamada da implementação deste simples método dependente de dispositivo. Por exemplo, o nível mais baixo do método pcap,

::

  void EnablePcap (std::string prefix, Ptr<NetDevice> nd, bool promiscuous = false, 
  		bool explicitFilename = false);

..
	will call the device implementation of ``EnablePcapInternal`` directly.  All
	other public pcap tracing methods build on this implementation to provide 
	additional user-level functionality.  What this means to the user is that all 
	device helpers in the system will have all of the pcap trace methods available;
	and these methods will all work in the same way across devices if the device 
	implements ``EnablePcapInternal`` correctly.

chamaremos diretamente a implementação do dispositivo de ``EnablePcapInternal``. Todos os outros métodos de rastreamento pcap públicos desta implementação são para prover funcionalidade adicional em nível de usuário. Para o usuário, isto significa que todas as classes assistentes de dispositivo no sistema terão todos os métodos de rastreamento pcap disponíveis; e estes métodos trabalharão da mesma forma entre dispositivos se o dispositivo implementar corretamente ``EnablePcapInternal``.

.. 
	Pcap Tracing Device Helper Methods

Métodos da Classe Assistente de Dispositivo para Rastreamento Pcap
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

::

  void EnablePcap (std::string prefix, Ptr<NetDevice> nd, 
  		bool promiscuous = false, bool explicitFilename = false);
  void EnablePcap (std::string prefix, std::string ndName, 
  		bool promiscuous = false, bool explicitFilename = false);
  void EnablePcap (std::string prefix, NetDeviceContainer d, 
  		bool promiscuous = false);
  void EnablePcap (std::string prefix, NodeContainer n, 
  		bool promiscuous = false);
  void EnablePcap (std::string prefix, uint32_t nodeid, uint32_t deviceid, 
  		bool promiscuous = false);
  void EnablePcapAll (std::string prefix, bool promiscuous = false);

..
	In each of the methods shown above, there is a default parameter called 
	``promiscuous`` that defaults to false.  This parameter indicates that the
	trace should not be gathered in promiscuous mode.  If you do want your traces
	to include all traffic seen by the device (and if the device supports a 
	promiscuous mode) simply add a true parameter to any of the calls above.  For example,

Em cada método apresentado existe um parâmetro padrão chamado ``promiscuous`` que é definido para o valor "false". Este parâmetro indica que o rastreamento não deveria coletar dados em modo promíscuo. Se quisermos incluir todo tráfego visto pelo dispositivo devemos modificar o valor para "true". Por exemplo,

::

  Ptr<NetDevice> nd;
  ...
  helper.EnablePcap ("prefix", nd, true);

..
	will enable promiscuous mode captures on the ``NetDevice`` specified by ``nd``.

ativará o modo de captura promíscuo no ``NetDevice`` especificado por ``nd``.

..
	The first two methods also include a default parameter called ``explicitFilename``
	that will be discussed below.

Os  dois primeiros métodos também incluem um parâmetro padrão chamado ``explicitFilename`` que será abordado a seguir.

..
	You are encouraged to peruse the Doxygen for class ``PcapHelperForDevice``
	to find the details of these methods; but to summarize ...

É interessante procurar maiores detalhes dos métodos da classe ``PcapHelperForDevice`` no Doxygen; mas para resumir ...

..
	You can enable pcap tracing on a particular node/net-device pair by providing a
	``Ptr<NetDevice>`` to an ``EnablePcap`` method.  The ``Ptr<Node>`` is 
	implicit since the net device must belong to exactly one ``Node``.
	For example, 

Podemos ativar o rastreamento pcap em um par nó/dispositivo-rede específico provendo um ``Ptr<NetDevice>`` para um método ``EnablePcap``. O ``Ptr<Node>`` é implícito, pois o dispositivo de rede deve estar em um ``Node``. Por exemplo,

::

  Ptr<NetDevice> nd;
  ...
  helper.EnablePcap ("prefix", nd);

..
	You can enable pcap tracing on a particular node/net-device pair by providing a
	``std::string`` representing an object name service string to an 
	``EnablePcap`` method.  The ``Ptr<NetDevice>`` is looked up from the name
	string.  Again, the ``<Node>`` is implicit since the named net device must 
	belong to exactly one ``Node``.  For example, 

Podemos ativar o rastreamento pcap em um par nó/dispositivo-rede passando uma ``std::string`` que representa um nome de serviço para um método ``EnablePcap``. O ``Ptr<NetDevice>`` é buscado a partir do nome da `string`.
Novamente, o ``Ptr<Node>`` é implícito pois o dispositivo de rede deve estar em um ``Node``. 

::

  Names::Add ("server" ...);
  Names::Add ("server/eth0" ...);
  ...
  helper.EnablePcap ("prefix", "server/eth0");

..
	You can enable pcap tracing on a collection of node/net-device pairs by 
	providing a ``NetDeviceContainer``.  For each ``NetDevice`` in the container
	the type is checked.  For each device of the proper type (the same type as is 
	managed by the device helper), tracing is enabled.    Again, the ``<Node>`` is 
	implicit since the found net device must belong to exactly one ``Node``.
	For example, 

Podemos ativar o rastreamento pcap em uma coleção de pares nós/dispositivos usando um ``NetDeviceContainer``. Para cada ``NetDevice`` no contêiner o tipo é verificado. Para cada dispositivo com o tipo adequado, o rastreamento será ativado. Por exemplo,

::

  NetDeviceContainer d = ...;
  ...
  helper.EnablePcap ("prefix", d);

..
	You can enable pcap tracing on a collection of node/net-device pairs by 
	providing a ``NodeContainer``.  For each ``Node`` in the ``NodeContainer``
	its attached ``NetDevices`` are iterated.  For each ``NetDevice`` attached
	to each node in the container, the type of that device is checked.  For each 
	device of the proper type (the same type as is managed by the device helper), 
	tracing is enabled.

Podemos ativar o rastreamento em uma coleção de pares nó/dispositivo-rede usando um ``NodeContainer``. Para cada ``Node`` no ``NodeContainer`` seus ``NetDevices`` são percorridos e verificados segundo o tipo. Para cada dispositivo com o tipo adequado, o rastreamento é ativado.

::

  NodeContainer n;
  ...
  helper.EnablePcap ("prefix", n);

..
	You can enable pcap tracing on the basis of node ID and device ID as well as
	with explicit ``Ptr``.  Each ``Node`` in the system has an integer node ID
	and each device connected to a node has an integer device ID.

Podemos ativar o rastreamento pcap usando o número identificador (`ID`) do nó e do dispositivo. Todo ``Node`` no sistema tem um valor inteiro indicando o `ID` do nó e todo dispositivo conectado ao nó tem um valor inteiro indicando o `ID` do dispositivo.

::

  helper.EnablePcap ("prefix", 21, 1);

..
	Finally, you can enable pcap tracing for all devices in the system, with the
	same type as that managed by the device helper.

Por fim, podemos ativar rastreamento pcap para todos os dispositivos no sistema, desde que o tipo seja o mesmo gerenciado pela classe assistentes de dispositivo.

::

  helper.EnablePcapAll ("prefix");

.. 
	Pcap Tracing Device Helper Filename Selection

Seleção de um Nome de Arquivo para o Rastreamento Pcap da Classe Assistente de Dispositivo
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

..
	Implicit in the method descriptions above is the construction of a complete 
	filename by the implementation method.  By convention, pcap traces in the 
	|ns3| system are of the form "<prefix>-<node id>-<device id>.pcap"

Implícito nas descrições de métodos anteriores é a construção do nome de arquivo por meio do método da implementação. Por convenção, rastreamento pcap no |ns3| usa a forma "<prefixo>-<id do nó>-<id do dispositivo>.pcap"

..
	As previously mentioned, every node in the system will have a system-assigned
	node id; and every device will have an interface index (also called a device id)
	relative to its node.  By default, then, a pcap trace file created as a result
	of enabling tracing on the first device of node 21 using the prefix "prefix"
	would be "prefix-21-1.pcap".
	
Como mencionado, todo nó no sistema terá um `id` de nó associado; e todo dispositivo terá um índice de interface (também chamado de id do dispositivo) relativo ao seu nó. Por padrão, então, um arquivo pcap criado como um resultado de ativar rastreamento no primeiro dispositivo do nó 21 usando o prefixo "prefix" seria "prefix-21-1.pcap".

..
	You can always use the |ns3| object name service to make this more clear.
	For example, if you use the object name service to assign the name "server"
	to node 21, the resulting pcap trace file name will automatically become,
	"prefix-server-1.pcap" and if you also assign the name "eth0" to the 
	device, your pcap file name will automatically pick this up and be called
	"prefix-server-eth0.pcap".

Sempre podemos usar o serviço de nome de objeto do |ns3| para tornar isso mais claro. Por exemplo, se você usa o serviço para associar o nome "server" ao nó 21, o arquivo pcap resultante automaticamente será, "prefix-server-1.pcap" e se você também associar o nome "eth0" ao dispositivo, seu nome do arquivo pcap automaticamente será denominado "prefix-server-eth0.pcap".

.. 
	Finally, two of the methods shown above,

Finalmente, dois dos métodos mostrados, 

::

  void EnablePcap (std::string prefix, Ptr<NetDevice> nd, 
  		bool promiscuous = false, bool explicitFilename = false);
  void EnablePcap (std::string prefix, std::string ndName, 
  		bool promiscuous = false, bool explicitFilename = false);

..
	have a default parameter called ``explicitFilename``.  When set to true,
	this parameter disables the automatic filename completion mechanism and allows
	you to create an explicit filename.  This option is only available in the 
	methods which enable pcap tracing on a single device.  

tem um parâmetro padrão ``explicitFilename``. Quando modificado para verdadeiro, este parâmetro desabilita o mecanismo automático de completar o nome do arquivo e permite criarmos um nome de arquivo abertamente. Esta opção está disponível nos métodos que ativam o rastreamento pcap em um único dispositivo.

..
	For example, in order to arrange for a device helper to create a single 
	promiscuous pcap capture file of a specific name ("my-pcap-file.pcap") on a
	given device, one could:

Por exemplo, com a finalidade providenciar uma classe assistente de dispositivo para criar um único arquivo de captura pcap no modo promíscuo com um nome específico ("my-pcap-file.pcap") em um determinado dispositivo:
	
::

  Ptr<NetDevice> nd;
  ...
  helper.EnablePcap ("my-pcap-file.pcap", nd, true, true);

..
	The first ``true`` parameter enables promiscuous mode traces and the second
	tells the helper to interpret the ``prefix`` parameter as a complete filename.

O primeiro parâmetro ``true`` habilita o modo de rastreamento promíscuo e o segundo faz com que o parâmetro ``prefix`` seja interpretado como um nome de arquivo completo.

.. 
	Ascii Tracing Device Helpers

Classes Assistentes de Dispositivo para Rastreamento ASCII
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

..
	The behavior of the ascii trace helper ``mixin`` is substantially similar to 
	the pcap version.  Take a look at ``src/network/helper/trace-helper.h`` if you want to 
	follow the discussion while looking at real code.

O comportamento do assistente  de rastreamento ASCII ``mixin`` é similar a versão do pcap. Acesse o arquivo ``src/network/helper/trace-helper.h`` para compreender melhor o funcionamento dessa classe assistente.

..
	The class ``AsciiTraceHelperForDevice`` adds the high level functionality for 
	using ascii tracing to a device helper class.  As in the pcap case, every device
	must implement a single virtual method inherited from the ascii trace ``mixin``.

A classe ``AsciiTraceHelperForDevice`` adiciona funcionalidade em alto nível para usar o rastreamento ASCII para uma classe assistente de dispositivo. Como no caso do pcap, todo dispositivo deve implementar um método herdado do rastreador ASCII ``mixin``.

::

  virtual void EnableAsciiInternal (Ptr<OutputStreamWrapper> stream,
		std::string prefix, Ptr<NetDevice> nd, bool explicitFilename) = 0;

..
	The signature of this method reflects the device-centric view of the situation
	at this level; and also the fact that the helper may be writing to a shared
	output stream.  All of the public ascii-trace-related methods inherited from 
	class ``AsciiTraceHelperForDevice`` reduce to calling this single device-
	dependent implementation method.  For example, the lowest level ascii trace
	methods,

A assinatura deste método reflete a visão do dispositivo da situação neste nível; e também o fato que o assistente pode ser escrito para um fluxo de saída compartilhado. Todos os métodos públicos associados ao rastreamento ASCII herdam da classe ``AsciiTraceHelperForDevice`` resumem-se a chamada deste único método dependente de implementação. Por exemplo, os métodos de rastreamento ASCII de mais baixo nível,

::

  void EnableAscii (std::string prefix, Ptr<NetDevice> nd, 
  		bool explicitFilename = false);
  void EnableAscii (Ptr<OutputStreamWrapper> stream, Ptr<NetDevice> nd);

.. 
	will call the device implementation of ``EnableAsciiInternal`` directly,
	providing either a valid prefix or stream.  All other public ascii tracing 
	methods will build on these low-level functions to provide additional user-level
	functionality.  What this means to the user is that all device helpers in the 
	system will have all of the ascii trace methods available; and these methods
	will all work in the same way across devices if the devices implement 
	``EnablAsciiInternal`` correctly.

chamarão uma implementação de ``EnableAsciiInternal`` diretamente, passando um prefixo ou fluxo válido. Todos os outros métodos públicos serão construídos a partir destas funções de baixo nível para fornecer funcionalidades adicionais em nível de usuário. Para o usuário, isso significa que todos os assistentes de
dispositivo no sistema terão todos os métodos de rastreamento ASCII disponíveis e estes métodos trabalharão do mesmo modo em todos os dispositivos se estes implementarem ``EnableAsciiInternal``.

.. 
	Ascii Tracing Device Helper Methods

Métodos da Classe Assistente de Dispositivo para Rastreamento  ASCII
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

::

  void EnableAscii (std::string prefix, Ptr<NetDevice> nd, 
  		bool explicitFilename = false);
  void EnableAscii (Ptr<OutputStreamWrapper> stream, Ptr<NetDevice> nd);

  void EnableAscii (std::string prefix, std::string ndName, 
  		bool explicitFilename = false);
  void EnableAscii (Ptr<OutputStreamWrapper> stream, std::string ndName);

  void EnableAscii (std::string prefix, NetDeviceContainer d);
  void EnableAscii (Ptr<OutputStreamWrapper> stream, NetDeviceContainer d);

  void EnableAscii (std::string prefix, NodeContainer n);
  void EnableAscii (Ptr<OutputStreamWrapper> stream, NodeContainer n);

  void EnableAsciiAll (std::string prefix);
  void EnableAsciiAll (Ptr<OutputStreamWrapper> stream);

  void EnableAscii (std::string prefix, uint32_t nodeid, uint32_t deviceid, 
  		bool explicitFilename);
  void EnableAscii (Ptr<OutputStreamWrapper> stream, uint32_t nodeid, 
  		uint32_t deviceid);

..
	You are encouraged to peruse the Doxygen for class ``AsciiTraceHelperForDevice``
	to find the details of these methods; but to summarize ...

Para maiores detalhes sobre os métodos é interessante consultar a documentação para a classe ``AsciiTraceHelperForDevice``; mas para resumir ...

..
	There are twice as many methods available for ascii tracing as there were for
	pcap tracing.  This is because, in addition to the pcap-style model where traces
	from each unique node/device pair are written to a unique file, we support a model
	in which trace information for many node/device pairs is written to a common file.
	This means that the <prefix>-<node>-<device> file name generation mechanism is 
	replaced by a mechanism to refer to a common file; and the number of API methods
	is doubled to allow all combinations.

Há duas vezes mais métodos disponíveis para rastreamento ASCII que para rastreamento pcap. Isto ocorre pois para o modelo pcap os rastreamentos de cada par nó/dispositivo-rede são escritos para um único arquivo, enquanto que no ASCII todo as as informações são escritas para um arquivo comum. Isto significa que o mecanismo de geração de nomes de arquivos `<prefixo>-<nó>-<dispositivo>` é substituído por um mecanismo para referenciar um arquivo comum; e o número de métodos da API é duplicado para permitir todas as combinações.

..
	Just as in pcap tracing, you can enable ascii tracing on a particular 
	node/net-device pair by providing a ``Ptr<NetDevice>`` to an ``EnableAscii``
	method.  The ``Ptr<Node>`` is implicit since the net device must belong to 
	exactly one ``Node``.  For example, 

Assim como no rastreamento pcap, podemos ativar o rastreamento ASCII em um par nó/dispositivo-rede passando um ``Ptr<NetDevice>`` para  um método ``EnableAscii``. O ``Ptr<Node>`` é implícito pois o dispositivo de rede deve pertencer a exatamente um ``Node``. Por exemplo,

::

  Ptr<NetDevice> nd;
  ...
  helper.EnableAscii ("prefix", nd);

..
	The first four methods also include a default parameter called ``explicitFilename``
	that operate similar to equivalent parameters in the pcap case.

Os primeiros quatro métodos também incluem um parâmetro padrão ``explicitFilename`` que opera similar aos parâmetros no caso do pcap.

..
	In this case, no trace contexts are written to the ascii trace file since they
	would be redundant.  The system will pick the file name to be created using
	the same rules as described in the pcap section, except that the file will
	have the suffix ".tr" instead of ".pcap".

Neste caso, nenhum contexto de rastreamento é escrito para o arquivo ASCII pois seriam redundantes. O sistema pegará o nome do arquivo para ser criado usando as mesmas regras como descritas na seção pcap, exceto que o arquivo terá o extensão ".tr" ao invés de ".pcap".

..
	If you want to enable ascii tracing on more than one net device and have all 
	traces sent to a single file, you can do that as well by using an object to
	refer to a single file.  We have already seen this in the "cwnd" example
	above:

Para habilitar o rastreamento ASCII em mais de um dispositivo de rede e ter todos os dados de rastreamento enviados para um único arquivo, pode-se usar um objeto para referenciar um único arquivo. Nós já verificamos isso no exemplo "cwnd":

::

  Ptr<NetDevice> nd1;
  Ptr<NetDevice> nd2;
  ...
  Ptr<OutputStreamWrapper> stream = asciiTraceHelper.CreateFileStream 
  		("trace-file-name.tr");
  ...
  helper.EnableAscii (stream, nd1);
  helper.EnableAscii (stream, nd2);

..
	In this case, trace contexts are written to the ascii trace file since they
	are required to disambiguate traces from the two devices.  Note that since the
	user is completely specifying the file name, the string should include the ",tr"
	for consistency.

Neste caso, os contextos são escritos para o arquivo ASCII quando é necessário distinguir os dados de rastreamento de dois dispositivos. É interessante usar no nome do arquivo a extensão ".tr" por motivos de consistência.

..
	You can enable ascii tracing on a particular node/net-device pair by providing a
	``std::string`` representing an object name service string to an 
	``EnablePcap`` method.  The ``Ptr<NetDevice>`` is looked up from the name
	string.  Again, the ``<Node>`` is implicit since the named net device must 
	belong to exactly one ``Node``.  For example, 

Podemos habilitar o rastreamento ASCII em um par nó/dispositivo-rede específico passando ao método ``EnableAscii`` uma ``std::string`` representando um nome no serviço de nomes de objetos. O ``Ptr<NetDevice>`` é obtido a partir do nome. Novamente, o ``<Node>`` é implícito pois o dispositivo de rede deve pertencer a exatamente um ``Node``. Por exemplo,

::

  Names::Add ("client" ...);
  Names::Add ("client/eth0" ...);
  Names::Add ("server" ...);
  Names::Add ("server/eth0" ...);
  ...
  helper.EnableAscii ("prefix", "client/eth0");
  helper.EnableAscii ("prefix", "server/eth0");

..
	This would result in two files named "prefix-client-eth0.tr" and 
	"prefix-server-eth0.tr" with traces for each device in the respective trace
	file.  Since all of the EnableAscii functions are overloaded to take a stream wrapper,
	you can use that form as well:

Isto resultaria em dois nomes de arquivos - "prefix-client-eth0.tr" e "prefix-server-eth0.tr" - com os rastreamentos de cada dispositivo em  seu arquivo respectivo. Como todas as funções do ``EnableAscii`` são sobrecarregadas para suportar um *stream wrapper*, podemos usar da seguinte forma também:

::

  Names::Add ("client" ...);
  Names::Add ("client/eth0" ...);
  Names::Add ("server" ...);
  Names::Add ("server/eth0" ...);
  ...
  Ptr<OutputStreamWrapper> stream = asciiTraceHelper.CreateFileStream 
  		("trace-file-name.tr");
  ...
  helper.EnableAscii (stream, "client/eth0");
  helper.EnableAscii (stream, "server/eth0");

..
	This would result in a single trace file called "trace-file-name.tr" that 
	contains all of the trace events for both devices.  The events would be 
	disambiguated by trace context strings.

Isto resultaria em um único arquivo chamado "trace-file-name.tr" que contém todosos eventos rastreados para ambos os dispositivos. Os eventos seriam diferenciados por `strings` de contexto.

..
	You can enable ascii tracing on a collection of node/net-device pairs by 
	providing a ``NetDeviceContainer``.  For each ``NetDevice`` in the container
	the type is checked.  For each device of the proper type (the same type as is 
	managed by the device helper), tracing is enabled.    Again, the ``<Node>`` is 
	implicit since the found net device must belong to exactly one ``Node``.
	For example, 

Podemos habilitar o rastreamento ASCII em um coleção de pares nó/dispositivo-rede fornecendo um ``NetDeviceContainer``. Para cada ``NetDevice`` no contêiner o tipo é verificado. Para cada dispositivo de um tipo adequado (o mesmo tipo que é gerenciado por uma classe assistente de dispositivo), o rastreamento é habilitado. Novamente, o ``<Node>`` é implícito pois o dispositivo de rede encontrado deve pertencer a exatamente um ``Node``. 

::

  NetDeviceContainer d = ...;
  ...
  helper.EnableAscii ("prefix", d);

..
	This would result in a number of ascii trace files being created, each of which
	follows the <prefix>-<node id>-<device id>.tr convention.  Combining all of the
	traces into a single file is accomplished similarly to the examples above:

Isto resultaria em vários arquivos de rastreamento ASCII sendo criados, cada um seguindo a convenção ``<prefixo>-<id do nó>-<id do dispositivo>.tr``.

Para obtermos um único arquivo teríamos:

::

  NetDeviceContainer d = ...;
  ...
  Ptr<OutputStreamWrapper> stream = asciiTraceHelper.CreateFileStream 
  		("trace-file-name.tr");
  ...
  helper.EnableAscii (stream, d);

..
	You can enable ascii tracing on a collection of node/net-device pairs by 
	providing a ``NodeContainer``.  For each ``Node`` in the ``NodeContainer``
	its attached ``NetDevices`` are iterated.  For each ``NetDevice`` attached
	to each node in the container, the type of that device is checked.  For each 
	device of the proper type (the same type as is managed by the device helper), 
	tracing is enabled.

Podemos habilitar o rastreamento ASCII em um coleção de pares nó/dispositivo-rede fornecendo um ``NodeContainer``. Para cada ``Node`` no ``NodeContainer``, os seus ``NetDevices`` são percorridos. Para cada ``NetDevice`` associado a cada nó no contêiner, o tipo do dispositivo é verificado. Para cada dispositivo do tipo adequado (o mesmo tipo que é gerenciado pelo assistente de dispositivo), o rastreamento é habilitado.

::

  NodeContainer n;
  ...
  helper.EnableAscii ("prefix", n);

..
	This would result in a number of ascii trace files being created, each of which
	follows the <prefix>-<node id>-<device id>.tr convention.  Combining all of the
	traces into a single file is accomplished similarly to the examples above:
		
Isto resultaria em vários arquivos ASCII sendo criados, cada um seguindo a convenção ``<prefixo>-<id do nó>-<id do dispositivo>.tr``.

..
	You can enable pcap tracing on the basis of node ID and device ID as well as
	with explicit ``Ptr``.  Each ``Node`` in the system has an integer node ID
	and each device connected to a node has an integer device ID.
	
Podemos habilitar o rastreamento pcap na base da `ID` do nó e `ID` do dispositivo tão bem como com um ``Ptr``. Cada ``Node`` no sistema possui um número identificador inteiro associado ao nó e cada dispositivo conectado possui um número identificador inteiro associado ao dispositivo.

::

  helper.EnableAscii ("prefix", 21, 1);

..
	Of course, the traces can be combined into a single file as shown above.

Os rastreamentos podem ser combinados em um único arquivo como mostrado acima.

..
	Finally, you can enable pcap tracing for all devices in the system, with the
	same type as that managed by the device helper.

Finalmente, podemos habilitar o rastreamento ASCII para todos os dispositivos no sistema.

::

  helper.EnableAsciiAll ("prefix");

..
	This would result in a number of ascii trace files being created, one for
	every device in the system of the type managed by the helper.  All of these
	files will follow the <prefix>-<node id>-<device id>.tr convention.  Combining
	all of the traces into a single file is accomplished similarly to the examples
	above.

Isto resultaria em vários arquivos ASCII sendo criados, um para cada dispositivo no sistema do tipo gerenciado pelo assistente. Todos estes arquivos seguiriam a convenção ``<prefixo>-<id do nó>-<id do dispositivo>.tr``.

.. 
	Ascii Tracing Device Helper Filename Selection

Selecionando Nome de Arquivo para as Saídas ASCII
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

..
	Implicit in the prefix-style method descriptions above is the construction of the
	complete filenames by the implementation method.  By convention, ascii traces
	in the |ns3| system are of the form "<prefix>-<node id>-<device id>.tr"

Implícito nas descrições de métodos anteriores é a construção do nome de arquivo por meio do método da implementação. Por convenção, rastreamento ASCII no |ns3| usa a forma "``<prefixo>-<id do nó>-<id do dispositivo>.tr``".

..
	As previously mentioned, every node in the system will have a system-assigned
	node id; and every device will have an interface index (also called a device id)
	relative to its node.  By default, then, an ascii trace file created as a result
	of enabling tracing on the first device of node 21, using the prefix "prefix",
	would be "prefix-21-1.tr".

Como mencionado, todo nó no sistema terá um `id` de nó associado; e todo dispositivo terá um índice de interface (também chamado de id do dispositivo) relativo ao seu nó. Por padrão, então, um arquivo ASCII criado como um resultado de ativar rastreamento no primeiro dispositivo do nó 21 usando o prefixo "prefix" seria "prefix-21-1.tr".

..
	You can always use the |ns3| object name service to make this more clear.
	For example, if you use the object name service to assign the name "server"
	to node 21, the resulting ascii trace file name will automatically become,
	"prefix-server-1.tr" and if you also assign the name "eth0" to the 
	device, your ascii trace file name will automatically pick this up and be called
	"prefix-server-eth0.tr".

Sempre podemos usar o serviço de nome de objeto do |ns3| para tornar isso mais claro. Por exemplo, se usarmos o serviço para associar o nome ``server`` ao nó 21, o arquivo ASCII resultante automaticamente será, ``prefix-server-1.tr`` e se também associarmos o nome ``eth0`` ao dispositivo, o nome do arquivo ASCII automaticamente será denominado ``prefix-server-eth0.tr``.

..
	Several of the methods have a default parameter called ``explicitFilename``.
	When set to true, this parameter disables the automatic filename completion 
	mechanism and allows you to create an explicit filename.  This option is only
	available in the methods which take a prefix and enable tracing on a single device.  
	
Diversos métodos tem um parâmetro padrão ``explicitFilename``. Quando modificado para verdadeiro, este parâmetro desabilita o mecanismo automático de completar o nome do arquivo e permite criarmos um nome de arquivo abertamente. Esta opção está disponível nos métodos que possuam um prefixo e ativem o rastreamento em um único dispositivo.

.. 
	Pcap Tracing Protocol Helpers

Classes Assistentes de Protocolo para Rastreamento Pcap
++++++++++++++++++++++++++++++++++++++++++++++++++++++++

..
	The goal of these ``mixins`` is to make it easy to add a consistent pcap trace
	facility to protocols.  We want all of the various flavors of pcap tracing to 
	work the same across all protocols, so the methods of these helpers are 
	inherited by stack helpers.  Take a look at ``src/network/helper/trace-helper.h``
	if you want to follow the discussion while looking at real code.

O objetivo destes ``mixins`` é facilitar a adição de um mecanismo consistente para da facilidade de rastreamento para protocolos. Queremos que todos os mecanismos de rastreamento para todos os protocolos operem de mesma forma, logo os métodos dessas classe assistentes são herdados por assistentes de pilha. Acesse ``src/network/helper/trace-helper.h`` para acompanhar o conteúdo discutido nesta seção.

..
	In this section we will be illustrating the methods as applied to the protocol
	``Ipv4``.  To specify traces in similar protocols, just substitute the
	appropriate type.  For example, use a ``Ptr<Ipv6>`` instead of a
	``Ptr<Ipv4>`` and call ``EnablePcapIpv6`` instead of ``EnablePcapIpv4``.

Nesta seção ilustraremos os métodos aplicados ao protocolo ``Ipv4``. Para especificar rastreamentos em protocolos similares, basta substituir pelo tipo apropriado. Por exemplo, use um ``Ptr<Ipv6>`` ao invés de um ``Ptr<Ipv4>`` e chame um ``EnablePcapIpv6`` ao invés de ``EnablePcapIpv4``.

..
	The class ``PcapHelperForIpv4`` provides the high level functionality for
	using pcap tracing in the ``Ipv4`` protocol.  Each protocol helper enabling these
	methods must implement a single virtual method inherited from this class.  There
	will be a separate implementation for ``Ipv6``, for example, but the only
	difference will be in the method names and signatures.  Different method names
	are required to disambiguate class ``Ipv4`` from ``Ipv6`` which are both 
	derived from class ``Object``, and methods that share the same signature.

A classe ``PcapHelperForIpv4`` provê funcionalidade de alto nível para usar rastreamento no protocolo ``Ipv4``. Cada classe assistente de protocolo devem implementar um método herdado desta. Haverá uma implementação separada para ``Ipv6``, por exemplo, mas a diferença será apenas nos nomes dos métodos e assinaturas. Nomes de métodos diferentes são necessário para distinguir a classe ``Ipv4`` da ``Ipv6``, pois ambas são derivadas da classe ``Object``, logo os métodos compartilham a mesma assinatura.

::

  virtual void EnablePcapIpv4Internal (std::string prefix, Ptr<Ipv4> ipv4, 
  		uint32_t interface, bool explicitFilename) = 0;

..
	The signature of this method reflects the protocol and interface-centric view 
	of the situation at this level.  All of the public methods inherited from class 
	``PcapHelperForIpv4`` reduce to calling this single device-dependent
	implementation method.  For example, the lowest level pcap method,

A assinatura desse método reflete a visão do protocolo e interface da situação neste nível. Todos os métodos herdados da classe ``PcapHelperForIpv4`` resumem-se a chamada deste único método dependente de dispositivo. Por exemplo, o método do pcap de mais baixo nível, 

::

  void EnablePcapIpv4 (std::string prefix, Ptr<Ipv4> ipv4, uint32_t interface, 
  		bool explicitFilename = false);

..
	will call the device implementation of ``EnablePcapIpv4Internal`` directly.
	All other public pcap tracing methods build on this implementation to provide 
	additional user-level functionality.  What this means to the user is that all 
	protocol helpers in the system will have all of the pcap trace methods 
	available; and these methods will all work in the same way across 
	protocols if the helper implements ``EnablePcapIpv4Internal`` correctly.

chamará a implementação de dispositivo de ``EnablePcapIpv4Internal`` diretamente. Todos os outros métodos públicos de rastreamento pcap  são construídos a partir desta implementação para prover funcionalidades adicionais em nível do usuário. Para o usuário, isto significa que todas as classes assistentes de dispositivo no sistema terão todos os métodos de rastreamento pcap disponíveis; e estes métodos trabalharão da mesma forma entre dispositivos se o dispositivo implementar corretamente ``EnablePcapIpv4Internal``.


.. 
	Pcap Tracing Protocol Helper Methods

Métodos da Classe Assistente de Protocolo para Rastreamento Pcap
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

..
	These methods are designed to be in one-to-one correspondence with the ``Node``-
	and ``NetDevice``- centric versions of the device versions.  Instead of
	``Node`` and ``NetDevice`` pair constraints, we use protocol and interface
	constraints.

Estes métodos são projetados para terem correspondência de um-para-um com o ``Node`` e ``NetDevice``. Ao invés de restrições de pares ``Node`` e ``NetDevice``, usamos restrições de protocolo e interface.

.. 
	Note that just like in the device version, there are six methods:

Note que como na versão de dispositivo, há seis métodos:

::

  void EnablePcapIpv4 (std::string prefix, Ptr<Ipv4> ipv4, uint32_t interface, 
  		bool explicitFilename = false);
  void EnablePcapIpv4 (std::string prefix, std::string ipv4Name, 
  		uint32_t interface, bool explicitFilename = false);
  void EnablePcapIpv4 (std::string prefix, Ipv4InterfaceContainer c);
  void EnablePcapIpv4 (std::string prefix, NodeContainer n);
  void EnablePcapIpv4 (std::string prefix, uint32_t nodeid, uint32_t interface, 
  		bool explicitFilename);
  void EnablePcapIpv4All (std::string prefix);

..
	You are encouraged to peruse the Doxygen for class ``PcapHelperForIpv4``
	to find the details of these methods; but to summarize ...

Para maiores detalhes sobre estes métodos é interessante consultar na documentação da classe ``PcapHelperForIpv4``, mas para resumir ...

..
	You can enable pcap tracing on a particular protocol/interface pair by providing a
	``Ptr<Ipv4>`` and ``interface`` to an ``EnablePcap`` method.  For example, 

Podemos habilitar o rastreamento pcap em um par protocolo/interface  passando um ``Ptr<Ipv4>`` e ``interface`` para um método ``EnablePcap``. Por exemplo,

::

  Ptr<Ipv4> ipv4 = node->GetObject<Ipv4> ();
  ...
  helper.EnablePcapIpv4 ("prefix", ipv4, 0);

..
	You can enable pcap tracing on a particular node/net-device pair by providing a
	``std::string`` representing an object name service string to an 
	``EnablePcap`` method.  The ``Ptr<Ipv4>`` is looked up from the name
	string.  For example, 

Podemos ativar o rastreamento pcap em um par protocolo/interface passando uma ``std::string`` que representa um nome de serviço para um método ``EnablePcapIpv4``. O ``Ptr<Ipv4>`` é buscado a partir do nome da `string`.
Por exemplo,

::

  Names::Add ("serverIPv4" ...);
  ...
  helper.EnablePcapIpv4 ("prefix", "serverIpv4", 1);

..
	You can enable pcap tracing on a collection of protocol/interface pairs by 
	providing an ``Ipv4InterfaceContainer``.  For each ``Ipv4`` / interface
	pair in the container the protocol type is checked.  For each protocol of the 
	proper type (the same type as is managed by the device helper), tracing is 
	enabled for the corresponding interface.  For example, 

Podemos ativar o rastreamento pcap em uma coleção de pares protocolo/interface usando um ``Ipv4InterfaceContainer``. Para cada par``Ipv4``/interface no contêiner o tipo do protocolo é verificado. Para cada protocolo do tipo adequado, o rastreamento é ativado para a interface correspondente. Por exemplo,


::

  NodeContainer nodes;
  ...
  NetDeviceContainer devices = deviceHelper.Install (nodes);
  ... 
  Ipv4AddressHelper ipv4;
  ipv4.SetBase ("10.1.1.0", "255.255.255.0");
  Ipv4InterfaceContainer interfaces = ipv4.Assign (devices);
  ...
  helper.EnablePcapIpv4 ("prefix", interfaces);

..
	You can enable pcap tracing on a collection of protocol/interface pairs by 
	providing a ``NodeContainer``.  For each ``Node`` in the ``NodeContainer``
	the appropriate protocol is found.  For each protocol, its interfaces are 
	enumerated and tracing is enabled on the resulting pairs.  For example,

Podemos ativar o rastreamento em uma coleção de pares protocolo/interface usando um ``NodeContainer``. Para cada ``Node`` no ``NodeContainer`` o protocolo apropriado é encontrado. Para cada protocolo, suas interfaces são enumeradas e o rastreamento é ativado nos pares resultantes. Por exemplo,

::

  NodeContainer n;
  ...
  helper.EnablePcapIpv4 ("prefix", n);

..
	You can enable pcap tracing on the basis of node ID and interface as well.  In
	this case, the node-id is translated to a ``Ptr<Node>`` and the appropriate
	protocol is looked up in the node.  The resulting protocol and interface are
	used to specify the resulting trace source.

Pode ativar o rastreamento pcap usando o número identificador do nó e da interface. Neste caso, o `ID` do nó é traduzido para um ``Ptr<Node>`` e o protocolo apropriado é buscado no nó. O protocolo e interface resultante são usados para especificar a origem do rastreamento resultante.


::

  helper.EnablePcapIpv4 ("prefix", 21, 1);

..
	Finally, you can enable pcap tracing for all interfaces in the system, with
	associated protocol being the same type as that managed by the device helper.

Por fim, podemos ativar rastreamento pcap para todas as interfaces no sistema, desde que o protocolo seja do mesmo tipo gerenciado pela classe assistente.


::

  helper.EnablePcapIpv4All ("prefix");

.. 
	Pcap Tracing Protocol Helper Filename Selection

Seleção de um Nome de Arquivo para o Rastreamento Pcap da Classe Assistente de Protocolo
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

..
	Implicit in all of the method descriptions above is the construction of the
	complete filenames by the implementation method.  By convention, pcap traces
	taken for devices in the |ns3| system are of the form 
	"<prefix>-<node id>-<device id>.pcap".  In the case of protocol traces,
	there is a one-to-one correspondence between protocols and ``Nodes``.
	This is because protocol ``Objects`` are aggregated to ``Node Objects``.
	Since there is no global protocol id in the system, we use the corresponding
	node id in file naming.  Therefore there is a possibility for file name 
	collisions in automatically chosen trace file names.  For this reason, the
	file name convention is changed for protocol traces.

Implícito nas descrições de métodos anterior é a construção do nome de arquivo por meio do método da implementação. Por convenção, rastreamento pcap no |ns3| usa a forma ``<prefixo>-<id do nó>-<id do dispositivo>.pcap``. No caso de rastreamento de protocolos, há uma correspondência de um-para-um entre protocolos e ``Nodes``. Isto porque ``Objects`` de protocolo são agregados a `Node Objects``. Como não há um `id` global de protocolo no sistema, usamos o `ID` do nó na nomeação do arquivo. Consequentemente há possibilidade de colisão de nomes quando usamos o sistema automático de nomes. Por esta razão, a convenção de nome de arquivo é modificada para rastreamentos de protocolos.

..
	As previously mentioned, every node in the system will have a system-assigned
	node id.  Since there is a one-to-one correspondence between protocol instances
	and node instances we use the node id.  Each interface has an interface id 
	relative to its protocol.  We use the convention 
	"<prefix>-n<node id>-i<interface id>.pcap" for trace file naming in protocol
	helpers.

Como mencionado, todo nó no sistema terá um `id` de nó associado. Como há uma correspondência de um-para-um entre instâncias de protocolo e instâncias de nó, usamos o `id` de nó. Cada interface tem um `id` de interface relativo ao seu protocolo. Usamos a convenção "<prefixo>-n<id do nó>-i<id da interface>.pcap" para especificar o nome do arquivo de rastreamento para as classes assistentes de protocolo.

..
	Therefore, by default, a pcap trace file created as a result of enabling tracing
	on interface 1 of the Ipv4 protocol of node 21 using the prefix "prefix"
	would be "prefix-n21-i1.pcap".

Consequentemente, por padrão, uma arquivo pcap criado como um resultado da ativação de rastreamento na interface 1 do protocolo ipv4 do nó 21 usando o prefixo ``prefix`` seria ``prefix-n21-i1.pcap``.

..
	You can always use the |ns3| object name service to make this more clear.
	For example, if you use the object name service to assign the name "serverIpv4"
	to the Ptr<Ipv4> on node 21, the resulting pcap trace file name will 
	automatically become, "prefix-nserverIpv4-i1.pcap".

Sempre podemos usar o serviço de nomes de objetos do |ns3| para tornar isso mais claro. Por exemplo, se usamos o serviço de nomes  para associar o nome "serverIpv4" ao Ptr<Ipv4> no nó 21, o nome de arquivo resultante seria ``prefix-nserverIpv4-i1.pcap``.

..
	Several of the methods have a default parameter called ``explicitFilename``.
	When set to true, this parameter disables the automatic filename completion 
	mechanism and allows you to create an explicit filename.  This option is only
	available in the methods which take a prefix and enable tracing on a single device.  

Diversos métodos tem um parâmetro padrão ``explicitFilename``. Quando modificado para verdadeiro, este parâmetro desabilita o mecanismo automático de completar o nome do arquivo e permite criarmos um nome de arquivo abertamente. Esta opção está disponível nos métodos que  ativam o rastreamento pcap em um único dispositivo.

.. 
	Ascii Tracing Protocol Helpers

Classes Assistentes de Protocolo para Rastreamento ASCII
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

..
	The behavior of the ascii trace helpers is substantially similar to the pcap
	case.  Take a look at ``src/network/helper/trace-helper.h`` if you want to 
	follow the discussion while looking at real code.

O comportamento dos assistentes de rastreamento ASCII é similar ao do pcap. Acesse o arquivo ``src/network/helper/trace-helper.h`` para compreender melhor o funcionamento dessa classe assistente.

..
	In this section we will be illustrating the methods as applied to the protocol
	``Ipv4``.  To specify traces in similar protocols, just substitute the
	appropriate type.  For example, use a ``Ptr<Ipv6>`` instead of a
	``Ptr<Ipv4>`` and call ``EnableAsciiIpv6`` instead of ``EnableAsciiIpv4``.

Nesta seção apresentamos os métodos aplicados ao protocolo ``Ipv4``. Para protocolos similares apenas substitua para o tipo apropriado. Por exemplo, use um ``Ptr<Ipv6>`` ao invés de um  ``Ptr<Ipv4>`` e chame ``EnableAsciiIpv6`` ao invés de ``EnableAsciiIpv4``.

..
	The class ``AsciiTraceHelperForIpv4`` adds the high level functionality
	for using ascii tracing to a protocol helper.  Each protocol that enables these
	methods must implement a single virtual method inherited from this class.

A classe ``AsciiTraceHelperForIpv4`` adiciona funcionalidade de alto nível para usar rastreamento ASCII para um assistente de protocolo. Todo protocolo que usa estes métodos deve implementar um método herdado desta classe. 

::

  virtual void EnableAsciiIpv4Internal (Ptr<OutputStreamWrapper> stream, 
                                        std::string prefix, 
                                        Ptr<Ipv4> ipv4, 
                                        uint32_t interface,
                                        bool explicitFilename) = 0;

..
	The signature of this method reflects the protocol- and interface-centric view 
	of the situation at this level; and also the fact that the helper may be writing
	to a shared output stream.  All of the public methods inherited from class 
	``PcapAndAsciiTraceHelperForIpv4`` reduce to calling this single device-
	dependent implementation method.  For example, the lowest level ascii trace
	methods,

A assinatura deste método reflete a visão central do protocolo e interface da situação neste nível; e também o fato que o assistente pode ser escrito para um fluxo de saída compartilhado. Todos os métodos públicos herdados desta classe ``PcapAndAsciiTraceHelperForIpv4`` resumem-se a chamada deste único método dependente de implementação. Por exemplo, os métodos de rastreamento ASCII de mais baixo nível,

::

  void EnableAsciiIpv4 (std::string prefix, Ptr<Ipv4> ipv4, uint32_t interface, 
  		bool explicitFilename = false);
  void EnableAsciiIpv4 (Ptr<OutputStreamWrapper> stream, Ptr<Ipv4> ipv4, 
  		uint32_t interface);

..
	will call the device implementation of ``EnableAsciiIpv4Internal`` directly,
	providing either the prefix or the stream.  All other public ascii tracing 
	methods will build on these low-level functions to provide additional user-level
	functionality.  What this means to the user is that all device helpers in the 
	system will have all of the ascii trace methods available; and these methods
	will all work in the same way across protocols if the protocols implement 
	``EnablAsciiIpv4Internal`` correctly.

chamarão uma implementação de ``EnableAsciiIpv4Internal`` diretamente, passando um prefixo ou fluxo válido. Todos os outros métodos públicos serão construídos a partir destas funções de baixo nível para fornecer funcionalidades adicionais em nível de usuário. Para o usuário, isso significa que todos os assistentes de protocolos no sistema terão todos os métodos de rastreamento ASCII disponíveis e estes métodos trabalharão do mesmo modo em todos os protocolos se estes implementarem ``EnableAsciiIpv4Internal``.


.. 
	Ascii Tracing Protocol Helper Methods

Métodos da Classe Assistente de Protocolo para Rastreamento ASCII
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

::

  void EnableAsciiIpv4 (std::string prefix, Ptr<Ipv4> ipv4, uint32_t interface, 
                        bool explicitFilename = false);
  void EnableAsciiIpv4 (Ptr<OutputStreamWrapper> stream, Ptr<Ipv4> ipv4,
                        uint32_t interface);

  void EnableAsciiIpv4 (std::string prefix, std::string ipv4Name, uint32_t interface,
                        bool explicitFilename = false);
  void EnableAsciiIpv4 (Ptr<OutputStreamWrapper> stream, std::string ipv4Name,
                        uint32_t interface);

  void EnableAsciiIpv4 (std::string prefix, Ipv4InterfaceContainer c);
  void EnableAsciiIpv4 (Ptr<OutputStreamWrapper> stream, Ipv4InterfaceContainer c);

  void EnableAsciiIpv4 (std::string prefix, NodeContainer n);
  void EnableAsciiIpv4 (Ptr<OutputStreamWrapper> stream, NodeContainer n);

  void EnableAsciiIpv4All (std::string prefix);
  void EnableAsciiIpv4All (Ptr<OutputStreamWrapper> stream);

  void EnableAsciiIpv4 (std::string prefix, uint32_t nodeid, uint32_t deviceid,
                        bool explicitFilename);
  void EnableAsciiIpv4 (Ptr<OutputStreamWrapper> stream, uint32_t nodeid, 
                        uint32_t interface);

..
	You are encouraged to peruse the Doxygen for class ``PcapAndAsciiHelperForIpv4``
	to find the details of these methods; but to summarize ...

Para maiores detalhes sobre os métodos consulte na documentação a classe ``PcapAndAsciiHelperForIpv4``; mas para resumir ...

..
	There are twice as many methods available for ascii tracing as there were for
	pcap tracing.  This is because, in addition to the pcap-style model where traces
	from each unique protocol/interface pair are written to a unique file, we 
	support a model in which trace information for many protocol/interface pairs is 
	written to a common file.  This means that the <prefix>-n<node id>-<interface>
	file name generation mechanism is replaced by a mechanism to refer to a common 
	file; and the number of API methods is doubled to allow all combinations.

Há duas vezes mais métodos disponíveis para rastreamento ASCII que para rastreamento pcap. Isto ocorre pois para o modelo pcap os rastreamentos de cada par protocolo/interface são escritos para um único arquivo, enquanto que no ASCII todo as as informações são escritas para um arquivo comum. Isto significa que o mecanismo de geração de nomes de arquivos "<prefixo>-n<id do nó>-i<interface>" é substituído por um mecanismo para referenciar um arquivo comum; e o número de métodos da API é duplicado para permitir todas as combinações.

..
	Just as in pcap tracing, you can enable ascii tracing on a particular 
	protocol/interface pair by providing a ``Ptr<Ipv4>`` and an ``interface``
	to an ``EnableAscii`` method.
	For example, 

Assim, como no rastreamento pcap, podemos ativar o rastreamento ASCII em um par protocolo/interface passando um ``Ptr<Ipv4>`` e uma ``interface`` para  um método ``EnableAsciiIpv4``. Por exemplo,


::

  Ptr<Ipv4> ipv4;
  ...
  helper.EnableAsciiIpv4 ("prefix", ipv4, 1);

..
	In this case, no trace contexts are written to the ascii trace file since they
	would be redundant.  The system will pick the file name to be created using
	the same rules as described in the pcap section, except that the file will
	have the suffix ".tr" instead of ".pcap".

Neste caso, nenhum contexto de rastreamento é escrito para o arquivo ASCII pois seriam redundantes. O sistema pegará o nome do arquivo para ser criado usando as mesmas regras como descritas na seção pcap, exceto que o arquivo terá o extensão ``.tr`` ao invés de ``.pcap``.

..
	If you want to enable ascii tracing on more than one interface and have all 
	traces sent to a single file, you can do that as well by using an object to
	refer to a single file.  We have already something similar to this in the
	"cwnd" example above:

Para habilitar o rastreamento ASCII em mais de uma interface e ter todos os dados de rastreamento enviados para um único arquivo, pode-se usar um objeto para referenciar um único arquivo. Nós já verificamos isso no exemplo "cwnd":

::

  Ptr<Ipv4> protocol1 = node1->GetObject<Ipv4> ();
  Ptr<Ipv4> protocol2 = node2->GetObject<Ipv4> ();
  ...
  Ptr<OutputStreamWrapper> stream = asciiTraceHelper.CreateFileStream 
  		("trace-file-name.tr");
  ...
  helper.EnableAsciiIpv4 (stream, protocol1, 1);
  helper.EnableAsciiIpv4 (stream, protocol2, 1);

..
	In this case, trace contexts are written to the ascii trace file since they
	are required to disambiguate traces from the two interfaces.  Note that since 
	the user is completely specifying the file name, the string should include the
	",tr" for consistency.

Neste caso, os contextos são escritos para o arquivo ASCII quando é necessário distinguir os dados de rastreamento de duas interfaces. É interessante usar no nome do arquivo a extensão ``.tr`` por motivos de consistência.

..
	You can enable ascii tracing on a particular protocol by providing a 
	``std::string`` representing an object name service string to an 
	``EnablePcap`` method.  The ``Ptr<Ipv4>`` is looked up from the name
	string.  The ``<Node>`` in the resulting filenames is implicit since there
	is a one-to-one correspondence between protocol instances and nodes,
	For example, 

Pode habilitar o rastreamento ASCII em protocolo específico passando ao método ``EnableAsciiIpv4`` uma ``std::string`` representando um nome no serviço de nomes de objetos. O ``Ptr<Ipv4>`` é obtido a partir do nome. O ``<Node>`` é implícito, pois há uma correspondência de um-para-um entre instancias de protocolos e nós. Por exemplo,

::

  Names::Add ("node1Ipv4" ...);
  Names::Add ("node2Ipv4" ...);
  ...
  helper.EnableAsciiIpv4 ("prefix", "node1Ipv4", 1);
  helper.EnableAsciiIpv4 ("prefix", "node2Ipv4", 1);

..
	This would result in two files named "prefix-nnode1Ipv4-i1.tr" and 
	"prefix-nnode2Ipv4-i1.tr" with traces for each interface in the respective 
	trace file.  Since all of the EnableAscii functions are overloaded to take a 
	stream wrapper, you can use that form as well:

Isto resultaria em dois nomes de arquivos ``prefix-nnode1Ipv4-i1.tr`` e ``prefix-nnode2Ipv4-i1.tr``, com os rastreamentos de cada interface em  seu arquivo respectivo. Como todas as funções do ``EnableAsciiIpv4`` são sobrecarregadas para suportar um *stream wrapper*, podemos usar da seguinte forma também:


::

  Names::Add ("node1Ipv4" ...);
  Names::Add ("node2Ipv4" ...);
  ...
  Ptr<OutputStreamWrapper> stream = asciiTraceHelper.CreateFileStream 
  		("trace-file-name.tr");
  ...
  helper.EnableAsciiIpv4 (stream, "node1Ipv4", 1);
  helper.EnableAsciiIpv4 (stream, "node2Ipv4", 1);

..
	This would result in a single trace file called "trace-file-name.tr" that 
	contains all of the trace events for both interfaces.  The events would be 
	disambiguated by trace context strings.

Isto resultaria em um único arquivo chamado ``trace-file-name.tr`` que contém todos os eventos rastreados para ambas as interfaces. Os eventos seriam diferenciados por `strings` de contexto.

.. 
	You can enable ascii tracing on a collection of protocol/interface pairs by 
	providing an ``Ipv4InterfaceContainer``.  For each protocol of the proper 
	type (the same type as is managed by the device helper), tracing is enabled
	for the corresponding interface.  Again, the ``<Node>`` is implicit since 
	there is a one-to-one correspondence between each protocol and its node.
	For example, 

Podemos habilitar o rastreamento ASCII em um coleção de pares protocolo/interface provendo um ``Ipv4InterfaceContainer``. Para cada protocolo no contêiner o tipo é verificado. Para cada protocolo do tipo adequado (o mesmo tipo que é gerenciado por uma classe assistente de protocolo), o rastreamento é habilitado para a interface correspondente. Novamente, o ``<Node>`` é implícito, pois há uma correspondência de um-para-um entre protocolo e seu nó. Por exemplo,


::

  NodeContainer nodes;
  ...
  NetDeviceContainer devices = deviceHelper.Install (nodes);
  ... 
  Ipv4AddressHelper ipv4;
  ipv4.SetBase ("10.1.1.0", "255.255.255.0");
  Ipv4InterfaceContainer interfaces = ipv4.Assign (devices);
  ...
  ...
  helper.EnableAsciiIpv4 ("prefix", interfaces);

..
	This would result in a number of ascii trace files being created, each of which
	follows the <prefix>-n<node id>-i<interface>.tr convention.  Combining all of the
	traces into a single file is accomplished similarly to the examples above:

Isto resultaria em vários arquivos de rastreamento ASCII sendo criados, cada um seguindo a convenção ``<prefixo>-n<id do nó>-i<interface>.tr``. 

Para obtermos um único arquivo teríamos:

::

  NodeContainer nodes;
  ...
  NetDeviceContainer devices = deviceHelper.Install (nodes);
  ... 
  Ipv4AddressHelper ipv4;
  ipv4.SetBase ("10.1.1.0", "255.255.255.0");
  Ipv4InterfaceContainer interfaces = ipv4.Assign (devices);
  ...
  Ptr<OutputStreamWrapper> stream = asciiTraceHelper.CreateFileStream 
  		("trace-file-name.tr");
  ...
  helper.EnableAsciiIpv4 (stream, interfaces);

..
	You can enable ascii tracing on a collection of protocol/interface pairs by 
	providing a ``NodeContainer``.  For each ``Node`` in the ``NodeContainer``
	the appropriate protocol is found.  For each protocol, its interfaces are 
	enumerated and tracing is enabled on the resulting pairs.  For example,

Podemos habilitar o rastreamento ASCII em uma coleção de pares protocolo/interface provendo um `NodeContainer``. Para cada ``Node`` no ``NodeContainer`` os protocolos apropriados são encontrados. Para cada protocolo, sua interface é enumerada e o rastreamento é habilitado nos pares. Por exemplo,

::

  NodeContainer n;
  ...
  helper.EnableAsciiIpv4 ("prefix", n);

..
	You can enable pcap tracing on the basis of node ID and device ID as well.  In
	this case, the node-id is translated to a ``Ptr<Node>`` and the appropriate
	protocol is looked up in the node.  The resulting protocol and interface are
	used to specify the resulting trace source.

Podemos habilitar o rastreamento pcap usando o número identificador do nó e número identificador do dispositivo. Neste caso, o `ID` do nó é traduzido para um ``Ptr<Node>`` e o protocolo apropriado é procurado no nó de rede. O protocolo e interface resultantes são usados para especificar a origem do rastreamento.

::

  helper.EnableAsciiIpv4 ("prefix", 21, 1);

.. 
	Of course, the traces can be combined into a single file as shown above.

Os rastreamentos podem ser combinados em um único arquivo como mostrado anteriormente.

.. 
	Finally, you can enable ascii tracing for all interfaces in the system, with
	associated protocol being the same type as that managed by the device helper.

Finalmente, podemos habilitar o rastreamento ASCII para todas as interfaces no sistema.

::

  helper.EnableAsciiIpv4All ("prefix");

..
	This would result in a number of ascii trace files being created, one for
	every interface in the system related to a protocol of the type managed by the
	helper.  All of these files will follow the <prefix>-n<node id>-i<interface.tr
	convention.  Combining all of the traces into a single file is accomplished 
	similarly to the examples above.

Isto resultaria em vários arquivos ASCII sendo criados, um para cada interface no sistema relacionada ao protocolo do tipo gerenciado pela classe assistente.Todos estes arquivos seguiriam a convenção
``<prefix>-n<id do node>-i<interface>.tr``.


.. 
	Ascii Tracing Protocol Helper Filename Selection

Seleção de Nome de Arquivo para Rastreamento ASCII da Classe Assistente de Protocolo 
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

..
	Implicit in the prefix-style method descriptions above is the construction of the
	complete filenames by the implementation method.  By convention, ascii traces
	in the |ns3| system are of the form "<prefix>-<node id>-<device id>.tr"

Implícito nas descrições de métodos anteriores é a construção do nome do arquivo por meio do método da implementação. Por convenção, rastreamento ASCII no sistema |ns3| são da forma ``<prefix>-<id node>-<id do dispositivo>.tr``.

..
	As previously mentioned, every node in the system will have a system-assigned
	node id.  Since there is a one-to-one correspondence between protocols and nodes
	we use to node-id to identify the protocol identity.  Every interface on a 
	given protocol will have an interface index (also called simply an interface) 
	relative to its protocol.  By default, then, an ascii trace file created as a result
	of enabling tracing on the first device of node 21, using the prefix "prefix",
	would be "prefix-n21-i1.tr".  Use the prefix to disambiguate multiple protocols
	per node.

Como mencionado, todo nó no sistema terá um número identificador de nó associado. Como há uma correspondência de um-para-um entre instâncias de protocolo e instâncias de nó, usamos o `ID` de nó. Cada interface em um protocolo terá um índice de interface (também chamando apenas de interface) relativo ao seu protocolo. Por padrão, então, um arquivo de rastreamento ASCII criado a partir do rastreamento no primeiro dispositivo do nó 21, usando o prefixo "prefix", seria ``prefix-n21-i1.tr``. O uso de prefixo distingue múltiplos protocolos por nó.

..
	You can always use the |ns3| object name service to make this more clear.
	For example, if you use the object name service to assign the name "serverIpv4"
	to the protocol on node 21, and also specify interface one, the resulting ascii 
	trace file name will automatically become, "prefix-nserverIpv4-1.tr".

Sempre podemos usar o serviço de nomes de objetos do |ns3| para tornar isso mais claro. Por exemplo, se usarmos o serviço de nomes para associar o nome "serverIpv4" ao Ptr<Ipv4> no nó 21, o nome de arquivo resultante seria ``prefix-nserverIpv4-i1.tr``.

..
	Several of the methods have a default parameter called ``explicitFilename``.
	When set to true, this parameter disables the automatic filename completion 
	mechanism and allows you to create an explicit filename.  This option is only
	available in the methods which take a prefix and enable tracing on a single device.  

Diversos métodos tem um parâmetro padrão ``explicitFilename``. Quando modificado para verdadeiro, este parâmetro desabilita o mecanismo automático de completar o nome do arquivo e permite criarmos um nome de arquivo abertamente. Esta opção está disponível nos métodos que  ativam o rastreamento em um único dispositivo.


.. 
	Summary

Considerações Finais
********************

..
	|ns3| includes an extremely rich environment allowing users at several 
	levels to customize the kinds of information that can be extracted from 
	simulations.  

O |ns3| inclui um ambiente completo para permitir usuários de diversos níveis  personalizar os tipos de informação para serem extraídas de suas simulações.

..
	There are high-level helper functions that allow users to simply control the 
	collection of pre-defined outputs to a fine granularity.  There are mid-level
	helper functions to allow more sophisticated users to customize how information
	is extracted and saved; and there are low-level core functions to allow expert
	users to alter the system to present new and previously unexported information
	in a way that will be immediately accessible to users at higher levels.

Existem funções assistentes de alto nível que permitem ao usuário o controle de um coleção de saídas predefinidas para uma granularidade mais fina. Existem funções assistentes de nível intermediário que permitem usuários mais sofisticados personalizar como as informações são extraídas e armazenadas; e existem funções de baixo nível que permitem usuários avançados alterarem o sistema para apresentar novas ou informações que não eram exportadas.

..
	This is a very comprehensive system, and we realize that it is a lot to 
	digest, especially for new users or those not intimately familiar with C++
	and its idioms.  We do consider the tracing system a very important part of
	|ns3| and so recommend becoming as familiar as possible with it.  It is
	probably the case that understanding the rest of the |ns3| system will
	be quite simple once you have mastered the tracing system

Este é um sistema muito abrangente e percebemos que é muita informação para digerir, especialmente para novos usuários ou aqueles que não estão intimamente familiarizados com C++ e suas expressões idiomáticas. Consideramos o sistema de rastreamento uma parte muito importante do |ns3|, assim recomendamos que familiarizem-se o máximo possível com ele. Compreender o restante do sistema |ns3| é bem simples, uma vez que dominamos o sistema de rastreamento.

