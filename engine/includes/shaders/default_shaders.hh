#pragma once

#include <unordered_map>
#include <vector>
#include <span>
#include <string>

static const std::array<unsigned char, 306> default_vert_shader = {0x23,0x76,0x65,0x72,0x73,0x69,0x6f,0x6e,0x20,0x34,0x36,0x30,0x20,0x63,0x6f,0x72,0x65,0x0d,0x0a,0x0d,0x0a,0x6c,0x61,0x79,0x6f,0x75,0x74,0x20,0x28,0x6c,0x6f,0x63,0x61,0x74,0x69,0x6f,0x6e,0x20,0x3d,0x20,0x30,0x29,0x20,0x69,0x6e,0x20,0x76,0x65,0x63,0x33,0x20,0x61,0x50,0x6f,0x73,0x3b,0x0d,0x0a,0x6c,0x61,0x79,0x6f,0x75,0x74,0x20,0x28,0x6c,0x6f,0x63,0x61,0x74,0x69,0x6f,0x6e,0x20,0x3d,0x20,0x31,0x29,0x20,0x69,0x6e,0x20,0x76,0x65,0x63,0x32,0x20,0x61,0x54,0x65,0x78,0x43,0x6f,0x6f,0x72,0x64,0x3b,0x0d,0x0a,0x0d,0x0a,0x6f,0x75,0x74,0x20,0x76,0x65,0x63,0x32,0x20,0x54,0x65,0x78,0x43,0x6f,0x6f,0x72,0x64,0x3b,0x0d,0x0a,0x0d,0x0a,0x75,0x6e,0x69,0x66,0x6f,0x72,0x6d,0x20,0x6d,0x61,0x74,0x34,0x20,0x6d,0x6f,0x64,0x65,0x6c,0x3b,0x0d,0x0a,0x75,0x6e,0x69,0x66,0x6f,0x72,0x6d,0x20,0x6d,0x61,0x74,0x34,0x20,0x76,0x69,0x65,0x77,0x3b,0x0d,0x0a,0x75,0x6e,0x69,0x66,0x6f,0x72,0x6d,0x20,0x6d,0x61,0x74,0x34,0x20,0x70,0x72,0x6f,0x6a,0x65,0x63,0x74,0x69,0x6f,0x6e,0x3b,0x0d,0x0a,0x0d,0x0a,0x76,0x6f,0x69,0x64,0x20,0x6d,0x61,0x69,0x6e,0x28,0x29,0x0d,0x0a,0x7b,0x0d,0x0a,0x20,0x20,0x20,0x20,0x67,0x6c,0x5f,0x50,0x6f,0x73,0x69,0x74,0x69,0x6f,0x6e,0x20,0x3d,0x20,0x70,0x72,0x6f,0x6a,0x65,0x63,0x74,0x69,0x6f,0x6e,0x20,0x2a,0x20,0x76,0x69,0x65,0x77,0x20,0x2a,0x20,0x6d,0x6f,0x64,0x65,0x6c,0x20,0x2a,0x20,0x76,0x65,0x63,0x34,0x28,0x61,0x50,0x6f,0x73,0x2c,0x20,0x31,0x2e,0x30,0x29,0x3b,0x0d,0x0a,0x20,0x20,0x20,0x20,0x54,0x65,0x78,0x43,0x6f,0x6f,0x72,0x64,0x20,0x3d,0x20,0x61,0x54,0x65,0x78,0x43,0x6f,0x6f,0x72,0x64,0x3b,0x0d,0x0a,0x7d,0x0d,0x0a,0x0d,0x0a,0x00};

static const std::array<unsigned char, 367> default_frag_shader = {0x23,0x76,0x65,0x72,0x73,0x69,0x6f,0x6e,0x20,0x34,0x36,0x30,0x20,0x63,0x6f,0x72,0x65,0x0d,0x0a,0x69,0x6e,0x20,0x76,0x65,0x63,0x32,0x20,0x54,0x65,0x78,0x43,0x6f,0x6f,0x72,0x64,0x3b,0x0d,0x0a,0x6f,0x75,0x74,0x20,0x76,0x65,0x63,0x34,0x20,0x46,0x72,0x61,0x67,0x43,0x6f,0x6c,0x6f,0x72,0x3b,0x0d,0x0a,0x0d,0x0a,0x75,0x6e,0x69,0x66,0x6f,0x72,0x6d,0x20,0x73,0x61,0x6d,0x70,0x6c,0x65,0x72,0x32,0x44,0x20,0x74,0x65,0x78,0x74,0x75,0x72,0x65,0x31,0x3b,0x0d,0x0a,0x75,0x6e,0x69,0x66,0x6f,0x72,0x6d,0x20,0x69,0x6e,0x74,0x20,0x75,0x73,0x65,0x54,0x65,0x78,0x74,0x75,0x72,0x65,0x3b,0x0d,0x0a,0x0d,0x0a,0x76,0x6f,0x69,0x64,0x20,0x6d,0x61,0x69,0x6e,0x28,0x29,0x0d,0x0a,0x7b,0x0d,0x0a,0x20,0x20,0x20,0x20,0x69,0x66,0x28,0x75,0x73,0x65,0x54,0x65,0x78,0x74,0x75,0x72,0x65,0x20,0x3d,0x3d,0x20,0x31,0x29,0x20,0x7b,0x0d,0x0a,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x76,0x65,0x63,0x34,0x20,0x74,0x65,0x78,0x43,0x6f,0x6c,0x6f,0x72,0x20,0x3d,0x20,0x74,0x65,0x78,0x74,0x75,0x72,0x65,0x28,0x74,0x65,0x78,0x74,0x75,0x72,0x65,0x31,0x2c,0x20,0x54,0x65,0x78,0x43,0x6f,0x6f,0x72,0x64,0x29,0x3b,0x0d,0x0a,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x69,0x66,0x28,0x74,0x65,0x78,0x43,0x6f,0x6c,0x6f,0x72,0x2e,0x61,0x20,0x3c,0x20,0x30,0x2e,0x31,0x29,0x0d,0x0a,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x64,0x69,0x73,0x63,0x61,0x72,0x64,0x3b,0x0d,0x0a,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x46,0x72,0x61,0x67,0x43,0x6f,0x6c,0x6f,0x72,0x20,0x3d,0x20,0x74,0x65,0x78,0x43,0x6f,0x6c,0x6f,0x72,0x3b,0x0d,0x0a,0x20,0x20,0x20,0x20,0x7d,0x20,0x65,0x6c,0x73,0x65,0x20,0x7b,0x0d,0x0a,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x46,0x72,0x61,0x67,0x43,0x6f,0x6c,0x6f,0x72,0x20,0x3d,0x20,0x76,0x65,0x63,0x34,0x28,0x31,0x2e,0x30,0x2c,0x20,0x30,0x2e,0x35,0x2c,0x20,0x30,0x2e,0x32,0x2c,0x20,0x31,0x2e,0x30,0x29,0x3b,0x0d,0x0a,0x20,0x20,0x20,0x20,0x7d,0x0d,0x0a,0x7d,0x00};


static std::unordered_map<std::string, std::vector<std::span<const unsigned char>>> ShadersPackage = {
    { "default", { default_vert_shader, default_frag_shader, }, },
 };
