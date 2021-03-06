#include "Graphics/16bitTileEngine/TileLayer.h"

#include "Content/ContentManager.h"

#include "Graphics/16bitTileEngine/Camera2D.h"

#include <algorithm>
#include <raylib.h>
#include <iostream>

using namespace bengite::graphics;
using namespace bengite::content;

TileLayer::TileLayer()
	: Layer(-1), _tileSets(ContentManager::getTileSet()) {
	
	
	
}

TileLayer::TileLayer(const array<Tile, ChunkLength * ChunkLength>& tiles) 
	: Layer(-1), _tiles(tiles), _tileSets(ContentManager::getTileSet()) {
	
	
	
}

TileLayer::TileLayer(const array<Tile, ChunkLength * ChunkLength>& tiles, 
		const shared_ptr<TileSet>& tileSets)
	: Layer(-1), _tiles(tiles), _tileSets(tileSets) {
		
		
}

TileLayer::TileLayer(const shared_ptr<TileSet>& tileSets)
	: Layer(-1), _tileSets(tileSets) {
		
		
}


void TileLayer::Update(const GameTime&) {
	
	
	
}

void TileLayer::Draw(const GameTime& gameTime, SpriteBatch& spriteBatch) {
	
	Point cameraPt = Camera2D::getCell();
	
	Point viewPt = Point::toCell((Vector2) {
		Camera2D::getPosition().x + Camera2D::getViewPort().width,
		Camera2D::getPosition().y + Camera2D::getViewPort().height
	});
	
	Point min = (Point) {
		std::max((ChunkLength - 1) * _cellPosition.x, cameraPt.x - 1),
		std::max((ChunkLength - 1) * _cellPosition.y , cameraPt.y - 1)
	};
	Point max = (Point) {
		std::min(viewPt.x + 1, (ChunkLength - 1) * (_cellPosition.x + 1)),
		std::min(viewPt.y + 1, (ChunkLength - 1) * (_cellPosition.y + 1))
	};

	// if(max.x <= 0 || max.y <= 0 || min.x >= ChunkLength || min.y >= ChunkLength)
	// return;
	
	Tile tile;
	Rectangle destination = (Rectangle) {0, 0, TileLength, TileLength};
	
	for(register int y = min.y; y < max.y; y++) {
		
		for(register int x = min.x; x < max.x; x++) {
			
			tile = getTile(x - (ChunkLength - 1) * _cellPosition.x, y - (ChunkLength - 1) * _cellPosition.y);
			
			
			
			if(tile.getTileSet() == -1 || tile.getIndex() == -1)
				continue;
			
			destination.x = x * TileLength;
			destination.y = y * TileLength;
			
			spriteBatch.Draw(
				*((_tileSets.get() + tile.getTileSet())->getTexture()),
				(_tileSets.get() + tile.getTileSet())->getSourceRec(tile.getIndex()),
				destination, (Vector2){0.0f, 0.0f}, 0.0f, WHITE);
		}
		
	}
	
}

Tile TileLayer::getTile(const int& x, const int& y) {
	
	return _tiles[y + x * ChunkLength];
	
}

void TileLayer::setTile(const int& x, const int& y, const Tile& tile) {
	
	_tiles[y + x * ChunkLength] = tile;
	
}

void TileLayer::setTile(const int& x, const int& y, const int& index, const int& tileSet) {
	
	_tiles[y + x * ChunkLength].setIndex(index);
	_tiles[y + x * ChunkLength].setTileSet(tileSet);
	
}

void TileLayer::setTiles(const array<Tile, ChunkLength * ChunkLength>& tiles) {
	
	_tiles = tiles;
	
}

Json::Value TileLayer::toJson(void) const {
	
	Json::Value layerJson(Json::arrayValue);
	for(int x = 0; x < ChunkLength; x++) {
		for(int y = 0; y < ChunkLength; y++) {
			
			layerJson.append(_tiles[y + x * ChunkLength].toJson());
			
		}
		
	}
	
	return layerJson;
	
}

void TileLayer::fromJson(Json::Value& layerJson) {
	
	for(int x = 0; x < ChunkLength; x++) {
		
		for(int y = 0; y < ChunkLength; y++) {
			
			_tiles[y + x * ChunkLength].fromJson(layerJson[y + x * ChunkLength]);
			
		}
		
	}
	
}