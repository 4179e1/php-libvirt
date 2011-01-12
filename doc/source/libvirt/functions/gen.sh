for i in `cat func.list`
do
FUNC_NAME=$i
PAGE_NAME=`echo $FUNC_NAME | tr _ -`

cat >$PAGE_NAME.xml <<EOF
<?xml version="1.0" encoding="utf-8"?>
<refentry xml:id="function.$PAGE_NAME" xmlns="http://docbook.org/ns/docbook" xmlns:xlink="http://www.w3.org/1999/xlink">
 <refnamediv>
  <refname>$FUNC_NAME</refname>
  <refpurpose>%SHORT_DESCRIPTION</refpurpose>
 </refnamediv>

 <refsect1 role="description">
  &reftitle.description;
  <methodsynopsis>
   <type>%RETURN_TYPE</type><methodname>$FUNC_NAME</methodname>
   <methodparam><type>%PARAM_TYPE</type><parameter>%PARAM_NAME</parameter></methodparam>
   <methodparam choice="opt"><type>%PARAM2_TYPE</type><parameter>%PARAM2_NAME</parameter><initializer>%INIT_VALUE</initializer></methodparam>
  </methodsynopsis>
  <para>
  %COMMET
  </para>
 </refsect1>

 <refsect1 role="parameters">
  &reftitle.parameters;
  <para>
   <variablelist>

    <varlistentry>
     <term><parameter>%PARAM_NAME</parameter></term>
     <listitem>
      <para>
      %PARAM_SHORT_DESCRIPTION
      </para>
     </listitem>
    </varlistentry>

    <varlistentry>
     <term><parameter>%PARAM2_NAME</parameter></term>
     <listitem>
      <para>
      %PARAM2_SHORT_DESCRIPTION
      </para>
     </listitem>
    </varlistentry>

   </variablelist>
  </para>
 </refsect1>

 <refsect1 role="returnvalues">
  &reftitle.returnvalues;
  <para>
  %RETURN_DESCRIPTION
  </para>
 </refsect1>

 <refsect1 role="example">
  &reftitle.examples;
  <para>
   <example>
    <title><function>$FUNC_NAME</function> example</title>
    <para>
     %EXAMPLE_DESCRIPTION
    </para>
    <programlisting role="php">
<![CDATA[
%EXAMPLE_BODY
]]>
      </programlisting>
      &example.outputs.similar;
      <screen>
<![CDATA[
#EXAMPLE_RESULT
]]>
    </screen>
   </example>
  </para>
 </refsect1>

 <refsect1 role="seealso">
  &reftitle.seealso;
  <para>
   <simplelist>
    <member><function>%REF_FUNC</function></member>
   </simplelist>
  </para>
 </refsect1>
</refentry>
EOF
done