#include "Precomp.h"

#define BLOCK_HAIR 0x52494148
#define BLOCK_HELM 0x4D4C4548
#define BLOCK_SKIN 0x4e494b53
#define HIDE_HAIR  0x01000000
#define HIDE_HAIR_OFFSET 40
#define HEADER_SIZE 12

bool ConvertHAIRToHELM( std::string filename, std::string name ) {
	// Open file.
	int fhandle = _open( filename.c_str(), O_RDWR|O_BINARY );
	if ( fhandle == -1 ) throw std::exception( "File could not be opened." );

	// Check file size.
	long size = _filelength( fhandle );
	if ( size < 12 ) {
		_close( fhandle );
		throw std::exception( "File is too short!" );
	}

	// Allocate data.
	unsigned char* data = (unsigned char*)malloc( size );
	if ( data == NULL ) {
		_close ( fhandle );
		throw std::exception( "Insufficient memory." );
	}

	// Read data.
	if ( _read( fhandle, data, size ) != size ) {
		_close( fhandle );
		throw std::exception( "Cannot read file." );
	}

	// Check header.
	if ( data[4] != 0x01 || data[5] != 0x00 && data[6] != 0x0c || data[7] != 0x00 ) {
		_close( fhandle );
		throw std::exception( "Cannot handle file." );
	}

	// Parse packets.
	long packets = data[8];
	if ( packets < 0 ) throw std::exception( "Invalid packet count." );
	for ( long i = 0; i < packets; i++ ) {
		// Get the packet data.
		long pos = ( i * 8 ) + HEADER_SIZE;
		long type = ((long)(*(long*) &data[pos]));
		long offset = ((long)(*(long*) &data[pos+sizeof(long)]));
		
		

		// Handle Skin Packets
		if ( type == BLOCK_SKIN ) {
			if ( offset + 52 > size ) throw std::exception( "Skin offset out of bounds." );
			for ( size_t npos = 0; npos < 32; npos++ ) data[offset+8+npos] = 0x00;
			for ( size_t npos = 0; npos < name.length(); npos++ ) data[offset+8+npos] = name[npos];
			continue;
		}

		// Handle Hair Packets
		if ( type == BLOCK_HAIR ) {
			if ( offset + 44 > size ) throw std::exception( "Hair offset out of bounds." );

			*((long*)(&data[pos])) = BLOCK_HELM;
			*((long*)(&data[offset])) = BLOCK_HELM;
//			for ( size_t npos = 0; npos < 32; npos++ ) data[offset+8+npos] = 0x00;
//			for ( size_t npos = 0; npos < name.length(); npos++ ) data[offset+8+npos] = name[npos];
			*((long*)(&data[offset+HIDE_HAIR_OFFSET])) = 1;
			continue;
		}
	}

	// Write back to file.
	_lseek( fhandle, 0L, 0 );
	if ( _write( fhandle, data, size ) != size ) {
		_close( fhandle );
		free( data );
		throw std::exception( "Cannot write file." );
	}

	// Finish up.
	_close( fhandle );
	free( data );
	return true;
}

int __cdecl main( int argc, char** argv ) {
	// Prepare.
	system( "CLS" );
	PrintfTextOut TextOut;

	// Main attempt.
	try {
		// Read the ini file.
		boost::program_options::options_description ini_desc;
		ini_desc.add_options()
			( "settings.module", "Module to load data from." )
			( "settings.ignoresingles", "Comment out single-variation lists." )
			( "paths.nwn2-install", "Neverwinter Nights 2 installation path." )
			( "paths.nwn2-home", "Neverwinter Nights 2 home directory." )
			( "paths.output", "Folder to save new helm files in." );
		std::ifstream ini_file( "HairToHelm.ini" );
		boost::program_options::variables_map ini;
		boost::program_options::store( boost::program_options::parse_config_file( ini_file, ini_desc, true ), ini );

		// Get the check module.
		if ( !ini.count( "settings.module" ) ) throw std::exception( "Module not set." );
		std::string ModuleName = ini["settings.module"].as<std::string>();

		// Get the NWN2 install location.
		if ( !ini.count( "paths.nwn2-install" ) ) throw std::exception( "NWN2 install location not set." );
		std::string PathNWN2Install = ini["paths.nwn2-install"].as<std::string>();
		if ( !boost::filesystem::exists( PathNWN2Install ) ) throw std::exception( "NWN2 install location does not exist." );

		// Get the NWN2 home location.
		if ( !ini.count( "paths.nwn2-home" ) ) throw std::exception( "NWN2 home location not set." );
		std::string PathNWN2Home = ini["paths.nwn2-home"].as<std::string>();
		if ( !boost::filesystem::exists( PathNWN2Home ) ) throw std::exception( "NWN2 home location does not exist." );

		// Get the ouput location.
		if ( !ini.count( "paths.output" ) ) throw std::exception( "Output location not set." );
		std::string PathOutput = ini["paths.output"].as<std::string>();
		if ( !boost::filesystem::exists( PathOutput ) ) throw std::exception( "Output location does not exist." );

		// Load the module.
		ResourceManager resources( &TextOut );
		TextOut.WriteText( "Loading '%s' ... ", ModuleName.c_str() );
		LoadModule( resources, ModuleName.c_str(), PathNWN2Home.c_str(), PathNWN2Install.c_str() );
		TextOut.WriteText( "success!" );

		// Establish the log.
		std::ofstream log( "HairToHelm.log" );

		// Gather hair models.
		TextOut.WriteText( "\nGathering hair models ... " );
		std::vector<NWN::ResRef32> HairModels;
		for ( ResourceManager::FileId i = resources.GetEncapsulatedFileCount(); i != 0; i-- ) {
			// Get the resource.
			NWN::ResRef32 ResRef;
			NWN::ResType ResType;
			if ( !resources.GetEncapsulatedFileEntry( ( i - 1 ), ResRef, ResType ) ) continue;

			// Is it a model?
			if ( ResType != NWN::ResMDB ) continue;

			// Get the model name.
			std::string ResName = resources.StrFromResRef( ResRef );
			stringToUpperCase( ResName);

			// Only include hair models.
			if ( ResName.find( "_HAIR" ) == std::string::npos ) continue;	// Filter out all hair models.
			if ( ResName.find( "_HAIR_" ) != std::string::npos ) continue;	// Filter out non-standard hairs.
			if ( ResName.substr( 0, 2 ) != "P_" ) continue;					// Skip all non-P_ files.

			HairModels.push_back( ResRef );
		}
		TextOut.WriteText( "found %u models.", HairModels.size() );

		// Export models.
		TextOut.WriteText( "\nExporting models ... " );
		std::vector<std::pair<std::string,std::string>> ModelFiles;
		for ( std::vector<NWN::ResRef32>::iterator i = HairModels.begin(); i < HairModels.end(); i++ ) {
			// Get the new model name.
			std::string OldName = resources.StrFromResRef( *i );
			std::string NewName = OldName;
			stringToLowerCase( NewName );
			NewName.insert( NewName.find( "_hair" ) + 5, "_helm" );

			// Extract file.
			std::string Target = PathOutput + "\\" + NewName + ".mdb";
			DemandResource32 dem( resources, *i, NWN::ResMDB );
			if ( boost::filesystem::exists( Target ) ) boost::filesystem::remove( Target );
			boost::filesystem::copy_file( dem.GetDemandedFileName().c_str(), Target.c_str() );

			// Store for later use.
			ModelFiles.push_back( std::pair<std::string,std::string>( Target, NewName ) );
		}
		TextOut.WriteText( "complete!" );

		// Convert models.
		TextOut.WriteText( "\nConverting models ... " );
		std::vector<std::string> Errors;
		for ( std::vector<std::pair<std::string,std::string>>::iterator i = ModelFiles.begin(); i < ModelFiles.end(); i++ ) {
			try {
				ConvertHAIRToHELM( i->first, i->second );
			} catch ( std::exception &e ) {
				Errors.push_back( i->first + " : " + e.what() );
			}
		}
		TextOut.WriteText( "complete!" );

		// Output errors.
		TextOut.WriteText( "\nWriting errors ... " );
		if ( !Errors.empty() ) log << "Errors: ";
		for ( std::vector<std::string>::iterator i = Errors.begin(); i < Errors.end(); i++ ) {
			log << *i << "\n";
		}
		TextOut.WriteText( "complete!" );
	} catch ( std::exception &e ) {
		TextOut.WriteText( "ERROR: '%s'", e.what() );
	}

	// Exit
	return 0;
}