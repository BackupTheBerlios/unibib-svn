/* SQL Tables */

set client_min_messages=WARNING;

SET search_path TO public,people,publications;


-- drop everything
drop table public.object cascade;
drop schema publications cascade;
drop schema people cascade;
drop domain language;


-- Objects are unique
create schema people;
create schema publications;


CREATE DOMAIN language AS varchar(6) check (value in ('fr','en'));

CREATE TABLE object (
  id serial not null,
  key varchar(250), -- A key (ie, for easier reference)
  constraint object_id primary key (id),
  constraint unique_key unique (key)
);

CREATE INDEX object_key ON object (key);

/* 
	Personnes
*/


-- A person

create table people.person (
  id bigint not null,
  firstname varchar(70) not null,
  lastname varchar(70) not null,
  home_page varchar(255), -- the home page
  email varchar(255), -- the email
  constraint person_id primary key (id),
  constraint id foreign key (id) references object(id) on update cascade on delete cascade
);

create index person_firstname on person(firstname);

-- An user

create table people.users (
  login varchar(15) not null,
  constraint id foreign key (id) references object(id) on update cascade on delete cascade,
  constraint login_unique unique (login),
  constraint users_id primary key (id)
) inherits(person);


/* 
	Publications

*/


create table publications.publication (
  id bigint NOT NULL PRIMARY KEY,
  title varchar(150) NOT NULL,
  language language,
  year smallint NOT NULL,
  month smallint,
  url varchar(200),
  constraint id foreign key (id) references object(id) on update cascade on delete cascade
);

create table publications.authors (
 id_author bigint not null references person(id) on update cascade on delete restrict,
 id_publication bigint not null,
 constraint authors_publication_id foreign key (id_publication) references publication(id) on update cascade on delete restrict,
 rank smallint not null,
 -- L'ordre d'un auteur est unique pour une publication
 constraint one_order unique(id_publication,rank),
 -- An author can only appears one time in a publication
 constraint one_time_author unique(id_author,id_publication) 
);

-- A journal
create table publications.journal (
  name varchar(150) NOT NULL,
  constraint journal_id primary key (id),
  constraint id foreign key (id) references object(id) on update cascade on delete cascade
) inherits(publication);

-- A journal article
create table publications.article (
  journal varchar(150),
  volume varchar(10),
  number varchar(10),
  pages point,
  constraint article_id primary key (id),
  constraint id foreign key (id) references object(id) on update cascade on delete cascade
) inherits(publication);

-- Conference proceedings (here, author = editor)
create table publications.proceedings (
  volume varchar(10),
  number varchar(10),
  place varchar(50),   
  constraint proceedings_id primary key (id),
  constraint id foreign key (id) references object(id) on update cascade on delete cascade
) inherits(publication);

-- Publication in proceedings
create table publications.inproceedings (
  id_proceedings bigint not null references proceedings(id),
  pages point, -- x=start page y=end page
  constraint inproceedings_id primary key (id),
  constraint id foreign key (id) references object(id) on update cascade on delete cascade,
  constraint pages check (pages is null or pages[0] <= pages[1])
) inherits(publication);

