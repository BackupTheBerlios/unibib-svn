/* SQL Tables */

set client_min_messages=WARNING;

-- drop everything
drop schema unibib cascade;

-- Objects are unique
create schema unibib;

SET search_path TO unibib;



CREATE DOMAIN language AS varchar(6) check (value in ('fr','en','es'));

CREATE TABLE objects (
  id bigint not null,
  constraint object_id primary key (id)
);


/* 
	Personnes
*/


-- A persons

create table persons (
  id bigint not null,
  firstname varchar(70) not null,
  middlename varchar(70) not null,
  lastname varchar(70) not null,
  home_page varchar(255), -- the home page
  email varchar(255), -- the email
  constraint nom_unique unique (firstname, lastname), 
  constraint person_id primary key (id),
  constraint id foreign key (id) references objects(id) on update cascade on delete cascade
);

create index person_firstname on persons(firstname);
create index person_name on persons(firstname,lastname);

-- An users

create domain id_user bigint;



-- groups (a hierarchy of groups)

CREATE DOMAIN id_group bigint;
CREATE TABLE groups (
  -- These fields are requested by the tree class
  id bigint NOT NULL default '0',
  pre bigint NOT NULL,
  post bigint NOT NULL,
  parent bigint,
  CONSTRAINT unique_pre PRIMARY KEY (pre), 
  CONSTRAINT valid_parent FOREIGN KEY (parent) REFERENCES groups(pre), 
  -- end of tree fields 
  
  name varchar(255) NOT NULL,   
  can_create_user bool NOT NULL DEFAULT 'n', 
  can_create_group bool NOT NULL DEFAULT 'n', 
  constraint unique_group UNIQUE (id)
);

CREATE SEQUENCE groups_seq;

create table users (
  id id_user NOT NULL,
  login varchar(15) not null,
  main_group id_group not null,
  password varchar(255) not null, 
  
  constraint id foreign key (id) references persons(id) on update cascade on delete cascade,
  constraint valid_group foreign key (main_group) references groups(id) on update cascade on delete restrict,
  constraint unique_login unique (login),
  constraint users_id primary key (id)
);

-- users' groups
CREATE TABLE users_groups (
   id_user id_user NOT NULL,
   id_group id_group NOT NULL,
  
   -- the users must exist  
   constraint is_user foreign key (id_user) references users(id) on update cascade on delete cascade,
   -- the group must exist 
   constraint is_group foreign key (id_group) references groups(id) on update cascade on delete cascade
);

-- All groups of one users
CREATE VIEW groups_of_users (id_user, id_group) AS (SELECT id_user,id_group from users_groups) UNION (SELECT id,main_group from users);

/* 
        Folders

*/

CREATE TABLE folders (
  -- These fields are requested by the tree class
  id bigint NOT NULL default '0',
  pre bigint NOT NULL,
  post bigint NOT NULL,
  parent bigint,
  CONSTRAINT unique_folder_pre PRIMARY KEY (pre), 
  CONSTRAINT valid_parent FOREIGN KEY (parent) REFERENCES folders(pre), 

  --   
  name varchar(255) NOT NULL,
  description text, 
  id_user bigint NOT NULL, -- the folder belongs to one users at least
  
  CONSTRAINT non_empty_name CHECK(length(name) > 0),
  CONSTRAINT unique_folder UNIQUE(id),
  -- children must have different names 
  CONSTRAINT unique_folder_name UNIQUE(parent, name)
);

CREATE UNIQUE INDEX folders_id ON folders(id);
CREATE SEQUENCE folders_seq;

-- group & users appartenance
CREATE table user_objects (
   id_user id_user NOT NULL,
   object_id bigint NOT NULL,
   constraint valid_user foreign key (id_user) references users(id) on update cascade on delete cascade,
   constraint valid_object foreign key (object_id) references objects(id) on update cascade on delete cascade,
   constraint user_object_pk primary key (id_user, object_id)
);
CREATE table group_objects (
   id_group bigint NOT NULL,
   object_id bigint NOT NULL,
   constraint valid_group foreign key (id_group) references groups(id) on update cascade on delete cascade,
   constraint valid_object foreign key (object_id) references objects(id) on update cascade on delete cascade,
   constraint group_object_pk primary key (id_group, object_id)
);

CREATE table user_folders (
   id_user bigint NOT NULL,
   folder_id bigint NOT NULL,
   constraint valid_user foreign key (id_user) references users(id) on update cascade on delete cascade,
   constraint valid_object foreign key (folder_id) references folders(id) on update cascade on delete cascade,
   constraint user_folders_pk primary key (id_user, folder_id)
);

CREATE table group_folders (
   id_group bigint NOT NULL,
   folder_id bigint NOT NULL,
   constraint valid_group foreign key (id_group) references groups(id) on update cascade on delete cascade,
   constraint valid_object foreign key (folder_id) references folders(id) on update cascade on delete cascade,
   constraint group_folder_pk primary key (id_group, folder_id)
);


/* Organisations
*/


/* 
	Publications

*/


create table publication (
  id bigint NOT NULL PRIMARY KEY,
  key varchar(250), -- A key (ie, for easier reference)
  title varchar(150) NOT NULL,
  language language,
  year smallint NOT NULL,
  month smallint,
  url varchar(200),
  constraint unique_key unique (key),
  constraint id foreign key (id) references objects(id) on update cascade on delete cascade
);
CREATE INDEX object_key ON publication(key);

create table authors (
 id_author bigint not null,
 id_publication bigint not null,
 rank smallint not null,

 -- The author must exit
 constraint is_author foreign key (id_author) references persons(id) on update cascade on delete restrict,
 -- The publication must exist 
 constraint is_publication foreign key (id_publication) references publication(id) on update cascade on delete restrict,
 -- There is only one given rank per article
 constraint publication_unique_rank unique(id_publication,rank),
 -- An author can only appears one time in a publication
 constraint publication_one_time_author unique(id_author,id_publication) 
);

-- A journal
create table journal (
  id bigint NOT NULL,
  name varchar(150) NOT NULL,
  constraint journal_id primary key (id),
  constraint is_a_publication foreign key (id) references publication(id) on update cascade on delete cascade
);

-- A journal article
create table article (
  id bigint NOT NULL,
  journal bigint NOT NULL,
  volume varchar(10),
  number varchar(10),
  pages point,
  
  constraint article_id primary key (id),
  constraint in_journal foreign key (journal) references journal(id) on update cascade on delete restrict, 
  constraint is_a_publication foreign key (id) references publication(id) on update cascade on delete cascade
);

-- Conference proceedings (here, author = editor)
create table proceedings (
  id bigint NOT NULL,
  volume varchar(10),
  number varchar(10),
  place varchar(50),   
  constraint proceedings_id primary key (id),
  constraint is_a_publication foreign key (id) references publication(id) on update cascade on delete cascade
);

-- Publication in proceedings
create table inproceedings (
  id bigint NOT NULL,
  id_proceedings bigint NOT NULL,
  pages point, -- x=start page y=end page
  
  constraint inproceedings_id primary key (id),
  constraint is_a_publication foreign key (id) references publication(id),
  constraint id foreign key (id) references publication(id) on update cascade on delete cascade,
  constraint pages check (pages is null or pages[0] <= pages[1])
);



/** Add some data (root users, root folder)
*/

INSERT INTO groups (id,pre,post,name,can_create_user,can_create_group) VALUES (1,1,1,'admin','y','y');

INSERT INTO objects (id) VALUES (1);
INSERT INTO persons (id,firstname,middlename,lastname) VALUES (1,'','','');
INSERT INTO users (id,login,password,main_group) VALUES (1,'root',md5('root'),1);

INSERT INTO folders (id,pre,post,name,id_user) VALUES (1,1,1,'root',1);


CREATE SEQUENCE objects_seq START WITH 2;
